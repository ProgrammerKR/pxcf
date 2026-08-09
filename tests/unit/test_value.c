#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "pxcf/pxcf.h"

static void test_value_creation() {
    PxcfValue* v1 = pxcf_value_new_integer(42);
    assert(pxcf_value_type(v1) == PXCF_VALUE_INTEGER);
    int64_t i;
    pxcf_value_get_integer(v1, &i);
    assert(i == 42);
    pxcf_value_free(v1);

    PxcfValue* v2 = pxcf_value_new_string("hello", 5);
    assert(pxcf_value_type(v2) == PXCF_VALUE_STRING);
    size_t len;
    const char* str = pxcf_value_get_string(v2, &len);
    assert(len == 5);
    assert(strcmp(str, "hello") == 0);
    pxcf_value_free(v2);
}

static void test_value_array() {
    PxcfValue* arr = pxcf_value_new_array();
    assert(arr != NULL);
    assert(pxcf_array_size(arr) == 0);

    pxcf_array_append(arr, pxcf_value_new_integer(1));
    pxcf_array_append(arr, pxcf_value_new_integer(2));

    assert(pxcf_array_size(arr) == 2);
    
    int64_t i;
    pxcf_value_get_integer(pxcf_array_get(arr, 1), &i);
    assert(i == 2);

    pxcf_value_free(arr);
}

static void test_value_object() {
    PxcfValue* obj = pxcf_value_new_object();
    assert(obj != NULL);
    assert(pxcf_object_size(obj) == 0);

    pxcf_object_set(obj, "key1", pxcf_value_new_integer(100));
    pxcf_object_set(obj, "key2", pxcf_value_new_bool(true));

    assert(pxcf_object_size(obj) == 2);
    
    PxcfValue* v = pxcf_object_get(obj, "key1");
    assert(v != NULL);
    int64_t i;
    pxcf_value_get_integer(v, &i);
    assert(i == 100);

    pxcf_value_free(obj);
}

void test_value() {
    printf("Running value tests...\n");
    test_value_creation();
    test_value_array();
    test_value_object();
    printf("Value tests passed.\n");
}
