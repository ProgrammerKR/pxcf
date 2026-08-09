#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "pxcf/pxcf.h"

static void test_parser_basic() {
    const char* source = "project { name: \"Test\" version: 1.0 debug: true }";
    PxcfError err;
    PxcfDocument* doc = pxcf_parse_string(source, strlen(source), &err);
    assert(doc != NULL);

    PxcfValue* root = pxcf_document_root(doc);
    assert(root != NULL);
    assert(pxcf_value_type(root) == PXCF_VALUE_OBJECT);

    PxcfValue* proj = pxcf_object_get(root, "project");
    assert(proj != NULL);
    assert(pxcf_value_type(proj) == PXCF_VALUE_OBJECT);

    PxcfValue* name = pxcf_object_get(proj, "name");
    assert(name != NULL);
    assert(strcmp(pxcf_value_get_string(name, NULL), "Test") == 0);

    PxcfValue* version = pxcf_object_get(proj, "version");
    assert(version != NULL);
    double ver_val;
    assert(pxcf_value_get_float(version, &ver_val));
    assert(ver_val == 1.0);

    PxcfValue* debug = pxcf_object_get(proj, "debug");
    assert(debug != NULL);
    bool debug_val;
    assert(pxcf_value_get_bool(debug, &debug_val));
    assert(debug_val == true);

    pxcf_document_free(doc);
}

static void test_parser_arrays() {
    const char* source = "list: [1, 2, 3]";
    PxcfError err;
    PxcfDocument* doc = pxcf_parse_string(source, strlen(source), &err);
    assert(doc != NULL);

    PxcfValue* root = pxcf_document_root(doc);
    PxcfValue* list = pxcf_object_get(root, "list");
    assert(list != NULL);
    assert(pxcf_value_type(list) == PXCF_VALUE_ARRAY);
    assert(pxcf_array_size(list) == 3);

    int64_t v;
    pxcf_value_get_integer(pxcf_array_get(list, 1), &v);
    assert(v == 2);

    pxcf_document_free(doc);
}

static void test_parser_errors() {
    const char* source = "project { name: \"Test\" name: \"Dup\" }";
    PxcfError err;
    PxcfDocument* doc = pxcf_parse_string(source, strlen(source), &err);
    assert(doc == NULL);
    assert(err.code == PXCF_ERROR_DUPLICATE_KEY);
}

void test_parser() {
    printf("Running parser tests...\n");
    test_parser_basic();
    test_parser_arrays();
    test_parser_errors();
    printf("Parser tests passed.\n");
}
