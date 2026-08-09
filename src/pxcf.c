#include "pxcf/pxcf.h"
#include "internal/pxcf_internal.h"
#include "internal/parser_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PxcfDocument* pxcf_parse_string(const char* source, size_t length, PxcfError* error) {
    if (!source) return NULL;
    return pxcf_parse_internal(source, length, error);
}

PxcfDocument* pxcf_load_file(const char* path, PxcfError* error) {
    if (!path) return NULL;

    FILE* file = fopen(path, "rb");
    if (!file) {
        if (error) {
            pxcf_error_init(error);
            error->code = PXCF_ERROR_IO;
            snprintf(error->message, sizeof(error->message), "Could not open file '%s'.", path);
        }
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if (file_size < 0) {
        fclose(file);
        if (error) {
            pxcf_error_init(error);
            error->code = PXCF_ERROR_IO;
            snprintf(error->message, sizeof(error->message), "Could not read file size '%s'.", path);
        }
        return NULL;
    }

    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        if (error) {
            pxcf_error_init(error);
            error->code = PXCF_ERROR_OUT_OF_MEMORY;
        }
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < (size_t)file_size) {
        free(buffer);
        fclose(file);
        if (error) {
            pxcf_error_init(error);
            error->code = PXCF_ERROR_IO;
            snprintf(error->message, sizeof(error->message), "Could not read file '%s'.", path);
        }
        return NULL;
    }

    buffer[bytes_read] = '\0';
    fclose(file);

    PxcfDocument* doc = pxcf_parse_internal(buffer, bytes_read, error);
    if (error && error->code != PXCF_SUCCESS) {
        strncpy(error->file_path, path, sizeof(error->file_path) - 1);
        error->file_path[sizeof(error->file_path) - 1] = '\0';
    }

    free(buffer);
    return doc;
}

void pxcf_document_free(PxcfDocument* document) {
    if (!document) return;
    pxcf_value_free(document->root);
    free(document);
}

PxcfValue* pxcf_document_root(PxcfDocument* document) {
    return document ? document->root : NULL;
}
