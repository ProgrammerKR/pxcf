#ifndef PXCF_PARSER_INTERNAL_H
#define PXCF_PARSER_INTERNAL_H

#include "internal/lexer_internal.h"
#include "pxcf/pxcf_error.h"
#include "pxcf/pxcf_value.h"

typedef struct {
    Lexer lexer;
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
    PxcfError* error_out;
    size_t depth;
    size_t max_depth;
} Parser;

PxcfDocument* pxcf_parse_internal(const char* source, size_t length, PxcfError* error);

#endif // PXCF_PARSER_INTERNAL_H
