#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "pxcf/pxcf.h"

static void test_serializer_basic() {
    PxcfValue* root = pxcf_value_new_object();
    pxcf_object_set(root, "name", pxcf_value_new_string("ProX", 4));
    pxcf_object_set(root, "version", pxcf_value_new_integer(1));

    char* output = NULL;
    PxcfError err;
    bool success = pxcf_serialize_string(root, &output, &err);
    assert(success);
    assert(output != NULL);

    assert(strstr(output, "name: \"ProX\"") != NULL);
    assert(strstr(output, "version: 1") != NULL);

    free(output);
    pxcf_value_free(root);
}

static void test_serializer_array() {
    PxcfValue* arr = pxcf_value_new_array();
    pxcf_array_append(arr, pxcf_value_new_integer(1));
    pxcf_array_append(arr, pxcf_value_new_integer(2));

    PxcfValue* root = pxcf_value_new_object();
    pxcf_object_set(root, "items", arr);

    char* output = NULL;
    PxcfError err;
    bool success = pxcf_serialize_string(root, &output, &err);
    assert(success);
    assert(output != NULL);

    assert(strstr(output, "items: [") != NULL);
    
    free(output);
    pxcf_value_free(root);
}

void test_serializer() {
    printf("Running serializer tests...\n");
    test_serializer_basic();
    test_serializer_array();
    printf("Serializer tests passed.\n");
}
