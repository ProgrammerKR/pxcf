#include "internal/parser_internal.h"
#include "internal/pxcf_internal.h"
#include "internal/value_internal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void error_at(Parser* parser, Token* token, const char* message, PxcfErrorCode code) {
    if (parser->panic_mode) return;
    parser->panic_mode = true;
    parser->had_error = true;

    if (parser->error_out) {
        parser->error_out->code = code;
        parser->error_out->line = token->line;
        parser->error_out->column = token->column;
        parser->error_out->offset = token->offset;
        strncpy(parser->error_out->message, message, sizeof(parser->error_out->message) - 1);
        parser->error_out->message[sizeof(parser->error_out->message) - 1] = '\0';
    }
}

static void error(Parser* parser, const char* message, PxcfErrorCode code) {
    error_at(parser, &parser->previous, message, code);
}

static void error_at_current(Parser* parser, const char* message, PxcfErrorCode code) {
    error_at(parser, &parser->current, message, code);
}

static void advance(Parser* parser) {
    parser->previous = parser->current;

    for (;;) {
        parser->current = pxcf_lexer_next_token(&parser->lexer);
        if (parser->current.type != TOKEN_ERROR) break;

        // Ensure proper error token message is handled. (Not implemented in lexer properly yet)
        error_at_current(parser, parser->current.start, PXCF_ERROR_UNEXPECTED_TOKEN); // Adjust code
    }
}

static void consume(Parser* parser, TokenType type, const char* message, PxcfErrorCode code) {
    if (parser->current.type == type) {
        advance(parser);
        return;
    }

    error_at_current(parser, message, code);
}

static bool check(Parser* parser, TokenType type) {
    return parser->current.type == type;
}

static bool match(Parser* parser, TokenType type) {
    if (!check(parser, type)) return false;
    advance(parser);
    return true;
}

static PxcfValue* parse_value(Parser* parser);

static PxcfValue* parse_array(Parser* parser) {
    PxcfValue* array = pxcf_value_new_array();
    if (!array) {
        error(parser, "Out of memory.", PXCF_ERROR_OUT_OF_MEMORY);
        return NULL;
    }

    if (!check(parser, TOKEN_RBRACKET)) {
        do {
            PxcfValue* element = parse_value(parser);
            if (element) {
                if (!pxcf_array_append(array, element)) {
                    error(parser, "Out of memory.", PXCF_ERROR_OUT_OF_MEMORY);
                    pxcf_value_free(element);
                }
            }
        } while (match(parser, TOKEN_COMMA));
    }

    consume(parser, TOKEN_RBRACKET, "Expected ']' after array elements.", PXCF_ERROR_UNTERMINATED_ARRAY);
    return array;
}

static char* extract_string(Parser* parser, Token* token) {
    // Basic unescaping. For 1.0, keep it simple but functional.
    if (token->type == TOKEN_STRING) {
        size_t len = token->length - 2; // remove quotes
        const char* start = token->start + 1;
        
        char* str = (char*)malloc(len + 1);
        if (!str) return NULL;
        
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            if (start[i] == '\\' && i + 1 < len) {
                i++;
                switch(start[i]) {
                    case 'n': str[j++] = '\n'; break;
                    case 'r': str[j++] = '\r'; break;
                    case 't': str[j++] = '\t'; break;
                    case '\\': str[j++] = '\\'; break;
                    case '"': str[j++] = '"'; break;
                    default: str[j++] = start[i]; break; // unsupported escape
                }
            } else {
                str[j++] = start[i];
            }
        }
        str[j] = '\0';
        return str;
    } else if (token->type == TOKEN_IDENTIFIER) {
        char* str = (char*)malloc(token->length + 1);
        if (str) {
            memcpy(str, token->start, token->length);
            str[token->length] = '\0';
        }
        return str;
    }
    return NULL;
}

static PxcfValue* parse_object_block(Parser* parser) {
    PxcfValue* object = pxcf_value_new_object();
    if (!object) {
        error(parser, "Out of memory.", PXCF_ERROR_OUT_OF_MEMORY);
        return NULL;
    }

    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        if (!match(parser, TOKEN_IDENTIFIER) && !match(parser, TOKEN_STRING)) {
            error_at_current(parser, "Expected property name.", PXCF_ERROR_UNEXPECTED_TOKEN);
            break;
        }

        Token key_token = parser->previous;
        char* key = extract_string(parser, &key_token);

        if (pxcf_object_get(object, key) != NULL) {
            error(parser, "Duplicate key.", PXCF_ERROR_DUPLICATE_KEY);
        }

        PxcfValue* value = NULL;

        if (match(parser, TOKEN_COLON)) {
            value = parse_value(parser);
        } else if (match(parser, TOKEN_LBRACE)) {
            value = parse_object_block(parser);
        } else {
            error_at_current(parser, "Expected ':' or '{' after property name.", PXCF_ERROR_UNEXPECTED_TOKEN);
        }

        if (key && value) {
            pxcf_object_set(object, key, value);
        } else if (value) {
            pxcf_value_free(value);
        }
        free(key);
        
        if (parser->had_error) break;
    }

    consume(parser, TOKEN_RBRACE, "Expected '}' after object members.", PXCF_ERROR_UNTERMINATED_OBJECT);
    return object;
}

static PxcfValue* parse_value(Parser* parser) {
    parser->depth++;
    if (parser->depth > parser->max_depth) {
        error_at_current(parser, "Maximum nesting depth exceeded.", PXCF_ERROR_DEPTH_LIMIT);
        parser->depth--;
        return NULL;
    }

    PxcfValue* value = NULL;

    if (match(parser, TOKEN_NULL)) {
        value = pxcf_value_new_null();
    } else if (match(parser, TOKEN_TRUE)) {
        value = pxcf_value_new_bool(true);
    } else if (match(parser, TOKEN_FALSE)) {
        value = pxcf_value_new_bool(false);
    } else if (match(parser, TOKEN_INTEGER)) {
        char buf[64];
        size_t len = parser->previous.length < 63 ? parser->previous.length : 63;
        memcpy(buf, parser->previous.start, len);
        buf[len] = '\0';
        int64_t i = strtoll(buf, NULL, 10);
        value = pxcf_value_new_integer(i);
    } else if (match(parser, TOKEN_FLOAT)) {
        char buf[128];
        size_t len = parser->previous.length < 127 ? parser->previous.length : 127;
        memcpy(buf, parser->previous.start, len);
        buf[len] = '\0';
        double d = strtod(buf, NULL);
        value = pxcf_value_new_float(d);
    } else if (match(parser, TOKEN_STRING)) {
        char* str = extract_string(parser, &parser->previous);
        if (str) {
            value = pxcf_value_new_string(str, strlen(str));
            free(str);
        }
    } else if (match(parser, TOKEN_LBRACKET)) {
        value = parse_array(parser);
    } else if (match(parser, TOKEN_LBRACE)) {
        value = parse_object_block(parser);
    } else {
        error_at_current(parser, "Expected value.", PXCF_ERROR_UNEXPECTED_TOKEN);
    }

    parser->depth--;
    return value;
}

PxcfDocument* pxcf_parse_internal(const char* source, size_t length, PxcfError* error_out) {
    Parser parser;
    pxcf_lexer_init(&parser.lexer, source, length);
    parser.had_error = false;
    parser.panic_mode = false;
    parser.error_out = error_out;
    parser.depth = 0;
    parser.max_depth = 128; // Default depth limit

    if (error_out) pxcf_error_init(error_out);

    advance(&parser);

    PxcfValue* root = pxcf_value_new_object();
    if (!root) {
        if (error_out) {
            error_out->code = PXCF_ERROR_OUT_OF_MEMORY;
        }
        return NULL;
    }

    while (!check(&parser, TOKEN_EOF)) {
        if (!match(&parser, TOKEN_IDENTIFIER) && !match(&parser, TOKEN_STRING)) {
            error_at_current(&parser, "Expected property name.", PXCF_ERROR_UNEXPECTED_TOKEN);
            break;
        }

        Token key_token = parser.previous;
        char* key = extract_string(&parser, &key_token);

        if (pxcf_object_get(root, key) != NULL) {
            error(&parser, "Duplicate key.", PXCF_ERROR_DUPLICATE_KEY);
        }

        PxcfValue* value = NULL;

        if (match(&parser, TOKEN_COLON)) {
            value = parse_value(&parser);
        } else if (match(&parser, TOKEN_LBRACE)) {
            value = parse_object_block(&parser);
        } else {
            error_at_current(&parser, "Expected ':' or '{' after property name.", PXCF_ERROR_UNEXPECTED_TOKEN);
        }

        if (key && value) {
            pxcf_object_set(root, key, value);
        } else if (value) {
            pxcf_value_free(value);
        }
        free(key);

        if (parser.had_error) break;
    }

    if (parser.had_error) {
        pxcf_value_free(root);
        return NULL;
    }

    PxcfDocument* doc = (PxcfDocument*)malloc(sizeof(PxcfDocument));
    if (doc) {
        doc->root = root;
    } else {
        pxcf_value_free(root);
    }
    return doc;
}
