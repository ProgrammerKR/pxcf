#ifndef PXCF_LEXER_INTERNAL_H
#define PXCF_LEXER_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TOKEN_EOF = 0,
    TOKEN_ERROR,

    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_INTEGER,
    TOKEN_FLOAT,

    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,

    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,

    TOKEN_COLON,
    TOKEN_COMMA
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    size_t length;
    size_t line;
    size_t column;
    size_t offset;
} Token;

typedef struct {
    const char* source_start;
    const char* start;
    const char* current;
    const char* end;
    size_t line;
    size_t column;
    size_t start_column;
} Lexer;

void pxcf_lexer_init(Lexer* lexer, const char* source, size_t length);
Token pxcf_lexer_next_token(Lexer* lexer);

#endif // PXCF_LEXER_INTERNAL_H
