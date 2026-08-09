#ifndef PXCF_H
#define PXCF_H

#include "pxcf/pxcf_version.h"
#include "pxcf/pxcf_error.h"
#include "pxcf/pxcf_value.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parses a PXCF string into a document.
 * 
 * @param source The UTF-8 string to parse.
 * @param length The length of the string, or 0 if null-terminated.
 * @param error Optional error out-parameter.
 * @return A newly allocated document, or NULL on error.
 */
PxcfDocument* pxcf_parse_string(const char* source, size_t length, PxcfError* error);

/**
 * @brief Parses a PXCF file into a document.
 * 
 * @param path The file path.
 * @param error Optional error out-parameter.
 * @return A newly allocated document, or NULL on error.
 */
PxcfDocument* pxcf_load_file(const char* path, PxcfError* error);

/**
 * @brief Frees a document and all its values.
 * 
 * @param document The document to free.
 */
void pxcf_document_free(PxcfDocument* document);

/**
 * @brief Gets the root value of the document (usually an object).
 * 
 * @param document The document.
 * @return The root value.
 */
PxcfValue* pxcf_document_root(PxcfDocument* document);

/**
 * @brief Serializes a value to a string buffer.
 * 
 * @param value The value to serialize.
 * @param out_str Pointer to a string pointer that will receive the serialized output. Must be freed with free().
 * @param error Optional error out-parameter.
 * @return True on success, false on error.
 */
bool pxcf_serialize_string(const PxcfValue* value, char** out_str, PxcfError* error);

/**
 * @brief Serializes a value to a file.
 * 
 * @param value The value to serialize.
 * @param path The file path.
 * @param error Optional error out-parameter.
 * @return True on success, false on error.
 */
bool pxcf_write_file(const PxcfValue* value, const char* path, PxcfError* error);

#ifdef __cplusplus
}
#endif

#endif // PXCF_H
