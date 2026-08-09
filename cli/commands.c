#include "commands.h"
#include "pxcf/pxcf.h"
#include <stdio.h>
#include <stdlib.h>

static void print_error(const PxcfError* error) {
    fprintf(stderr, "PXCF validation failed.\n\n");
    if (error->file_path[0]) {
        fprintf(stderr, "File: %s\n", error->file_path);
    }
    fprintf(stderr, "Line: %zu\n", error->line);
    fprintf(stderr, "Column: %zu\n\n", error->column);
    
    fprintf(stderr, "Error [%s]\n", pxcf_error_code_string(error->code));
    fprintf(stderr, "%s\n", error->message);
}

int pxcf_command_check(const char* file_path) {
    PxcfError error;
    pxcf_error_init(&error);
    
    PxcfDocument* doc = pxcf_load_file(file_path, &error);
    if (!doc) {
        print_error(&error);
        return 1;
    }
    
    printf("'%s' is valid.\n", file_path);
    pxcf_document_free(doc);
    return 0;
}

int pxcf_command_format(const char* file_path, bool write_in_place) {
    PxcfError error;
    pxcf_error_init(&error);
    
    PxcfDocument* doc = pxcf_load_file(file_path, &error);
    if (!doc) {
        print_error(&error);
        return 1;
    }
    
    char* str = NULL;
    if (!pxcf_serialize_string(pxcf_document_root(doc), &str, &error)) {
        fprintf(stderr, "Serialization failed: %s\n", error.message);
        pxcf_document_free(doc);
        return 1;
    }
    
    if (write_in_place) {
        if (!pxcf_write_file(pxcf_document_root(doc), file_path, &error)) {
            fprintf(stderr, "Write failed: %s\n", error.message);
            free(str);
            pxcf_document_free(doc);
            return 1;
        }
        printf("Formatted '%s'.\n", file_path);
    } else {
        printf("%s\n", str);
    }
    
    free(str);
    pxcf_document_free(doc);
    return 0;
}

static void print_value(PxcfValue* value, int indent) {
    if (!value) {
        printf("null\n");
        return;
    }
    switch (pxcf_value_type(value)) {
        case PXCF_VALUE_NULL: printf("null\n"); break;
        case PXCF_VALUE_BOOL: {
            bool b;
            pxcf_value_get_bool(value, &b);
            printf("%s\n", b ? "true" : "false");
            break;
        }
        case PXCF_VALUE_INTEGER: {
            int64_t i;
            pxcf_value_get_integer(value, &i);
            printf("%lld\n", (long long)i);
            break;
        }
        case PXCF_VALUE_FLOAT: {
            double d;
            pxcf_value_get_float(value, &d);
            printf("%f\n", d);
            break;
        }
        case PXCF_VALUE_STRING: {
            printf("\"%s\"\n", pxcf_value_get_string(value, NULL));
            break;
        }
        case PXCF_VALUE_ARRAY: {
            printf("Array (size %zu)\n", pxcf_array_size(value));
            break;
        }
        case PXCF_VALUE_OBJECT: {
            printf("Object (size %zu)\n", pxcf_object_size(value));
            break;
        }
    }
}

int pxcf_command_inspect(const char* file_path) {
    PxcfError error;
    pxcf_error_init(&error);
    
    PxcfDocument* doc = pxcf_load_file(file_path, &error);
    if (!doc) {
        print_error(&error);
        return 1;
    }
    
    printf("Root: ");
    print_value(pxcf_document_root(doc), 0);
    
    pxcf_document_free(doc);
    return 0;
}
