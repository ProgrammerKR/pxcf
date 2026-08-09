#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "internal/lexer_internal.h"

static void test_lexer_basic() {
    const char* source = "project { name: \"Test\" }";
    Lexer lexer;
    pxcf_lexer_init(&lexer, source, strlen(source));

    Token t1 = pxcf_lexer_next_token(&lexer);
    assert(t1.type == TOKEN_IDENTIFIER);
    assert(t1.length == 7);
    assert(strncmp(t1.start, "project", 7) == 0);
    assert(t1.line == 1);

    Token t2 = pxcf_lexer_next_token(&lexer);
    assert(t2.type == TOKEN_LBRACE);

    Token t3 = pxcf_lexer_next_token(&lexer);
    assert(t3.type == TOKEN_IDENTIFIER);
    assert(strncmp(t3.start, "name", 4) == 0);

    Token t4 = pxcf_lexer_next_token(&lexer);
    assert(t4.type == TOKEN_COLON);

    Token t5 = pxcf_lexer_next_token(&lexer);
    assert(t5.type == TOKEN_STRING);
    assert(strncmp(t5.start, "\"Test\"", 6) == 0);

    Token t6 = pxcf_lexer_next_token(&lexer);
    assert(t6.type == TOKEN_RBRACE);

    Token t7 = pxcf_lexer_next_token(&lexer);
    assert(t7.type == TOKEN_EOF);
}

static void test_lexer_numbers() {
    const char* source = "42 -7 3.14 -0.5 1e6";
    Lexer lexer;
    pxcf_lexer_init(&lexer, source, strlen(source));

    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_INTEGER);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_INTEGER);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_FLOAT);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_FLOAT);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_FLOAT);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_EOF);
}

static void test_lexer_keywords() {
    const char* source = "true false null";
    Lexer lexer;
    pxcf_lexer_init(&lexer, source, strlen(source));

    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_TRUE);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_FALSE);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_NULL);
    assert(pxcf_lexer_next_token(&lexer).type == TOKEN_EOF);
}

void test_lexer() {
    printf("Running lexer tests...\n");
    test_lexer_basic();
    test_lexer_numbers();
    test_lexer_keywords();
    printf("Lexer tests passed.\n");
}
