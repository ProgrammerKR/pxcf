#ifndef PXCF_ERROR_H
#define PXCF_ERROR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PXCF_SUCCESS = 0,
    PXCF_ERROR_OUT_OF_MEMORY,
    PXCF_ERROR_IO,
    PXCF_ERROR_INVALID_UTF8,
    PXCF_ERROR_UNEXPECTED_TOKEN,
    PXCF_ERROR_INVALID_NUMBER,
    PXCF_ERROR_UNTERMINATED_STRING,
    PXCF_ERROR_UNTERMINATED_OBJECT,
    PXCF_ERROR_UNTERMINATED_ARRAY,
    PXCF_ERROR_DUPLICATE_KEY,
    PXCF_ERROR_INVALID_ESCAPE,
    PXCF_ERROR_INVALID_IDENTIFIER,
    PXCF_ERROR_DEPTH_LIMIT
} PxcfErrorCode;

typedef struct PxcfError {
    PxcfErrorCode code;
    size_t line;
    size_t column;
    size_t offset;
    char message[256];
    char file_path[256];
} PxcfError;

/**
 * @brief Initialize an error struct to a success state.
 */
void pxcf_error_init(PxcfError* error);

/**
 * @brief Get a string representation of an error code.
 */
const char* pxcf_error_code_string(PxcfErrorCode code);

#ifdef __cplusplus
}
#endif

#endif // PXCF_ERROR_H
