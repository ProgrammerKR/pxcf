#include <stdio.h>

extern void test_lexer();
extern void test_parser();
extern void test_value();
extern void test_serializer();

int main() {
    printf("Running unit tests...\n");
    test_lexer();
    test_parser();
    test_value();
    test_serializer();
    printf("All tests passed.\n");
    return 0;
}
