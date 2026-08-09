#include "pxcf/pxcf_error.h"
#include <string.h>

void pxcf_error_init(PxcfError* error) {
    if (error) {
        error->code = PXCF_SUCCESS;
        error->line = 0;
        error->column = 0;
        error->offset = 0;
        error->message[0] = '\0';
        error->file_path[0] = '\0';
    }
}

const char* pxcf_error_code_string(PxcfErrorCode code) {
    switch (code) {
        case PXCF_SUCCESS: return "PXCF_SUCCESS";
        case PXCF_ERROR_OUT_OF_MEMORY: return "PXCF_ERROR_OUT_OF_MEMORY";
        case PXCF_ERROR_IO: return "PXCF_ERROR_IO";
        case PXCF_ERROR_INVALID_UTF8: return "PXCF_ERROR_INVALID_UTF8";
        case PXCF_ERROR_UNEXPECTED_TOKEN: return "PXCF_ERROR_UNEXPECTED_TOKEN";
        case PXCF_ERROR_INVALID_NUMBER: return "PXCF_ERROR_INVALID_NUMBER";
        case PXCF_ERROR_UNTERMINATED_STRING: return "PXCF_ERROR_UNTERMINATED_STRING";
        case PXCF_ERROR_UNTERMINATED_OBJECT: return "PXCF_ERROR_UNTERMINATED_OBJECT";
        case PXCF_ERROR_UNTERMINATED_ARRAY: return "PXCF_ERROR_UNTERMINATED_ARRAY";
        case PXCF_ERROR_DUPLICATE_KEY: return "PXCF_ERROR_DUPLICATE_KEY";
        case PXCF_ERROR_INVALID_ESCAPE: return "PXCF_ERROR_INVALID_ESCAPE";
        case PXCF_ERROR_INVALID_IDENTIFIER: return "PXCF_ERROR_INVALID_IDENTIFIER";
        case PXCF_ERROR_DEPTH_LIMIT: return "PXCF_ERROR_DEPTH_LIMIT";
        default: return "UNKNOWN_ERROR";
    }
}
