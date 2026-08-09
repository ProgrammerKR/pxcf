#include "internal/lexer_internal.h"
#include <string.h>
#include <stdbool.h>

static bool is_at_end(Lexer* lexer) {
    return lexer->current >= lexer->end;
}

static char advance(Lexer* lexer) {
    if (is_at_end(lexer)) return '\0';
    lexer->current++;
    lexer->column++;
    return lexer->current[-1];
}

static char peek(Lexer* lexer) {
    if (is_at_end(lexer)) return '\0';
    return *lexer->current;
}

static char peek_next(Lexer* lexer) {
    if (lexer->current + 1 >= lexer->end) return '\0';
    return lexer->current[1];
}

static bool match(Lexer* lexer, char expected) {
    if (is_at_end(lexer)) return false;
    if (*lexer->current != expected) return false;
    lexer->current++;
    lexer->column++;
    return true;
}

static Token make_token(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->start;
    token.length = (size_t)(lexer->current - lexer->start);
    token.line = lexer->line;
    // column corresponds to the start of the token
    token.column = lexer->start_column;
    token.offset = (size_t)(lexer->start - lexer->source_start);
    return token;
}

static Token error_token(Lexer* lexer, const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = strlen(message);
    token.line = lexer->line;
    token.column = lexer->start_column;
    token.offset = (size_t)(lexer->start - lexer->source_start);
    return token;
}

void pxcf_lexer_init(Lexer* lexer, const char* source, size_t length) {
    lexer->source_start = source;
    lexer->start = source;
    lexer->current = source;
    if (length == 0 && source != NULL) {
        lexer->end = source + strlen(source);
    } else {
        lexer->end = source + length;
    }
    lexer->line = 1;
    lexer->column = 1;
    lexer->start_column = 1;
}

static void skip_whitespace(Lexer* lexer) {
    for (;;) {
        char c = peek(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                lexer->column = 0; // Will be 1 after advance if we did it here, but we just set to 0. advance() does ++
                advance(lexer); // advance() will increment column to 1
                break;
            case '#':
                // comment goes to end of line
                while (peek(lexer) != '\n' && !is_at_end(lexer)) {
                    advance(lexer);
                }
                break;
            default:
                return;
        }
    }
}

static Token string(Lexer* lexer) {
    while (peek(lexer) != '"' && !is_at_end(lexer)) {
        if (peek(lexer) == '\n') {
            lexer->line++;
            lexer->column = 0;
        } else if (peek(lexer) == '\\') {
            // skip the escaped character so we don't accidentally close the string
            // if it's a quote \"
            advance(lexer);
        }
        advance(lexer);
    }

    if (is_at_end(lexer)) {
        return error_token(lexer, "Unterminated string.");
    }

    // The closing quote.
    advance(lexer);
    return make_token(lexer, TOKEN_STRING);
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static Token number(Lexer* lexer) {
    TokenType type = TOKEN_INTEGER;

    while (is_digit(peek(lexer))) advance(lexer);

    // Look for a fractional part.
    if (peek(lexer) == '.' && is_digit(peek_next(lexer))) {
        type = TOKEN_FLOAT;
        advance(lexer); // Consume the "."

        while (is_digit(peek(lexer))) advance(lexer);
    }

    // Look for exponent part
    if (peek(lexer) == 'e' || peek(lexer) == 'E') {
        type = TOKEN_FLOAT;
        advance(lexer); // Consume the "e" or "E"

        if (peek(lexer) == '+' || peek(lexer) == '-') {
            advance(lexer);
        }

        if (!is_digit(peek(lexer))) {
            return error_token(lexer, "Invalid number format (missing exponent digits).");
        }

        while (is_digit(peek(lexer))) advance(lexer);
    }

    return make_token(lexer, type);
}

static TokenType check_keyword(Lexer* lexer, size_t start, size_t length, const char* rest, TokenType type) {
    if ((size_t)(lexer->current - lexer->start) == start + length &&
        memcmp(lexer->start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Lexer* lexer) {
    switch (lexer->start[0]) {
        case 'f': return check_keyword(lexer, 1, 4, "alse", TOKEN_FALSE);
        case 'n': return check_keyword(lexer, 1, 3, "ull", TOKEN_NULL);
        case 't': return check_keyword(lexer, 1, 3, "rue", TOKEN_TRUE);
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer* lexer) {
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer)) || peek(lexer) == '-') {
        advance(lexer);
    }
    return make_token(lexer, identifier_type(lexer));
}

Token pxcf_lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    
    lexer->start = lexer->current;
    lexer->start_column = lexer->column;

    if (is_at_end(lexer)) return make_token(lexer, TOKEN_EOF);

    char c = advance(lexer);

    if (is_alpha(c)) return identifier(lexer);
    if (is_digit(c)) return number(lexer);

    switch (c) {
        case '{': return make_token(lexer, TOKEN_LBRACE);
        case '}': return make_token(lexer, TOKEN_RBRACE);
        case '[': return make_token(lexer, TOKEN_LBRACKET);
        case ']': return make_token(lexer, TOKEN_RBRACKET);
        case ':': return make_token(lexer, TOKEN_COLON);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case '-': return number(lexer);
        case '"': return string(lexer);
    }

    return error_token(lexer, "Unexpected character.");
}
