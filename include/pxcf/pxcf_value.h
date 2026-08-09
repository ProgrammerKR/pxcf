#ifndef PXCF_VALUE_H
#define PXCF_VALUE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PXCF_VALUE_NULL,
    PXCF_VALUE_BOOL,
    PXCF_VALUE_INTEGER,
    PXCF_VALUE_FLOAT,
    PXCF_VALUE_STRING,
    PXCF_VALUE_ARRAY,
    PXCF_VALUE_OBJECT
} PxcfValueType;

typedef struct PxcfValue PxcfValue;
typedef struct PxcfDocument PxcfDocument;

// Value Creation
PxcfValue* pxcf_value_new_null(void);
PxcfValue* pxcf_value_new_bool(bool b);
PxcfValue* pxcf_value_new_integer(int64_t i);
PxcfValue* pxcf_value_new_float(double f);
PxcfValue* pxcf_value_new_string(const char* s, size_t len);
PxcfValue* pxcf_value_new_array(void);
PxcfValue* pxcf_value_new_object(void);
void pxcf_value_free(PxcfValue* value);

// Value Inspection
PxcfValueType pxcf_value_type(const PxcfValue* value);
bool pxcf_value_is_null(const PxcfValue* value);
bool pxcf_value_get_bool(const PxcfValue* value, bool* out_val);
bool pxcf_value_get_integer(const PxcfValue* value, int64_t* out_val);
bool pxcf_value_get_float(const PxcfValue* value, double* out_val);
const char* pxcf_value_get_string(const PxcfValue* value, size_t* out_len);

// Array API
size_t pxcf_array_size(const PxcfValue* array);
PxcfValue* pxcf_array_get(const PxcfValue* array, size_t index);
bool pxcf_array_append(PxcfValue* array, PxcfValue* value);

// Object API
size_t pxcf_object_size(const PxcfValue* object);
PxcfValue* pxcf_object_get(const PxcfValue* object, const char* key);
PxcfValue* pxcf_object_get_index(const PxcfValue* object, size_t index, const char** out_key);
bool pxcf_object_set(PxcfValue* object, const char* key, PxcfValue* value);

#ifdef __cplusplus
}
#endif

#endif // PXCF_VALUE_H
