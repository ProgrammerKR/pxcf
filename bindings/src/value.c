#include "pxcf/pxcf_value.h"
#include "internal/value_internal.h"
#include <stdlib.h>
#include <string.h>

static PxcfValue* allocate_value(PxcfValueType type) {
    PxcfValue* value = (PxcfValue*)malloc(sizeof(PxcfValue));
    if (value) {
        value->type = type;
        memset(&value->as, 0, sizeof(value->as));
    }
    return value;
}

PxcfValue* pxcf_value_new_null(void) {
    return allocate_value(PXCF_VALUE_NULL);
}

PxcfValue* pxcf_value_new_bool(bool b) {
    PxcfValue* value = allocate_value(PXCF_VALUE_BOOL);
    if (value) value->as.boolean = b;
    return value;
}

PxcfValue* pxcf_value_new_integer(int64_t i) {
    PxcfValue* value = allocate_value(PXCF_VALUE_INTEGER);
    if (value) value->as.integer = i;
    return value;
}

PxcfValue* pxcf_value_new_float(double f) {
    PxcfValue* value = allocate_value(PXCF_VALUE_FLOAT);
    if (value) value->as.floating = f;
    return value;
}

PxcfValue* pxcf_value_new_string(const char* s, size_t len) {
    PxcfValue* value = allocate_value(PXCF_VALUE_STRING);
    if (!value) return NULL;

    value->as.string.chars = (char*)malloc(len + 1);
    if (!value->as.string.chars) {
        free(value);
        return NULL;
    }

    if (s && len > 0) {
        memcpy(value->as.string.chars, s, len);
    }
    value->as.string.chars[len] = '\0';
    value->as.string.length = len;
    
    return value;
}

PxcfValue* pxcf_value_new_array(void) {
    PxcfValue* value = allocate_value(PXCF_VALUE_ARRAY);
    if (value) {
        value->as.array.elements = NULL;
        value->as.array.size = 0;
        value->as.array.capacity = 0;
    }
    return value;
}

PxcfValue* pxcf_value_new_object(void) {
    PxcfValue* value = allocate_value(PXCF_VALUE_OBJECT);
    if (value) {
        value->as.object.entries = NULL;
        value->as.object.size = 0;
        value->as.object.capacity = 0;
    }
    return value;
}

void pxcf_value_free(PxcfValue* value) {
    if (!value) return;

    switch (value->type) {
        case PXCF_VALUE_STRING:
            free(value->as.string.chars);
            break;
        case PXCF_VALUE_ARRAY:
            for (size_t i = 0; i < value->as.array.size; i++) {
                pxcf_value_free(value->as.array.elements[i]);
            }
            free(value->as.array.elements);
            break;
        case PXCF_VALUE_OBJECT:
            for (size_t i = 0; i < value->as.object.size; i++) {
                free(value->as.object.entries[i].key);
                pxcf_value_free(value->as.object.entries[i].value);
            }
            free(value->as.object.entries);
            break;
        default:
            break;
    }

    free(value);
}

PxcfValueType pxcf_value_type(const PxcfValue* value) {
    return value ? value->type : PXCF_VALUE_NULL;
}

bool pxcf_value_is_null(const PxcfValue* value) {
    return value == NULL || value->type == PXCF_VALUE_NULL;
}

bool pxcf_value_get_bool(const PxcfValue* value, bool* out_val) {
    if (value && value->type == PXCF_VALUE_BOOL) {
        if (out_val) *out_val = value->as.boolean;
        return true;
    }
    return false;
}

bool pxcf_value_get_integer(const PxcfValue* value, int64_t* out_val) {
    if (value && value->type == PXCF_VALUE_INTEGER) {
        if (out_val) *out_val = value->as.integer;
        return true;
    }
    return false;
}

bool pxcf_value_get_float(const PxcfValue* value, double* out_val) {
    if (value && value->type == PXCF_VALUE_FLOAT) {
        if (out_val) *out_val = value->as.floating;
        return true;
    }
    return false;
}

const char* pxcf_value_get_string(const PxcfValue* value, size_t* out_len) {
    if (value && value->type == PXCF_VALUE_STRING) {
        if (out_len) *out_len = value->as.string.length;
        return value->as.string.chars;
    }
    return NULL;
}

size_t pxcf_array_size(const PxcfValue* array) {
    if (array && array->type == PXCF_VALUE_ARRAY) {
        return array->as.array.size;
    }
    return 0;
}

PxcfValue* pxcf_array_get(const PxcfValue* array, size_t index) {
    if (array && array->type == PXCF_VALUE_ARRAY) {
        if (index < array->as.array.size) {
            return array->as.array.elements[index];
        }
    }
    return NULL;
}

bool pxcf_array_append(PxcfValue* array, PxcfValue* value) {
    if (!array || array->type != PXCF_VALUE_ARRAY || !value) return false;

    if (array->as.array.capacity < array->as.array.size + 1) {
        size_t new_cap = array->as.array.capacity == 0 ? 8 : array->as.array.capacity * 2;
        PxcfValue** new_elems = (PxcfValue**)realloc(array->as.array.elements, new_cap * sizeof(PxcfValue*));
        if (!new_elems) return false;
        array->as.array.elements = new_elems;
        array->as.array.capacity = new_cap;
    }

    array->as.array.elements[array->as.array.size++] = value;
    return true;
}

size_t pxcf_object_size(const PxcfValue* object) {
    if (object && object->type == PXCF_VALUE_OBJECT) {
        return object->as.object.size;
    }
    return 0;
}

PxcfValue* pxcf_object_get(const PxcfValue* object, const char* key) {
    if (!object || object->type != PXCF_VALUE_OBJECT || !key) return NULL;

    for (size_t i = 0; i < object->as.object.size; i++) {
        if (strcmp(object->as.object.entries[i].key, key) == 0) {
            return object->as.object.entries[i].value;
        }
    }
    return NULL;
}

PxcfValue* pxcf_object_get_index(const PxcfValue* object, size_t index, const char** out_key) {
    if (!object || object->type != PXCF_VALUE_OBJECT) return NULL;

    if (index < object->as.object.size) {
        if (out_key) *out_key = object->as.object.entries[index].key;
        return object->as.object.entries[index].value;
    }
    return NULL;
}

static char* duplicate_string(const char* s) {
    size_t len = strlen(s);
    char* dup = (char*)malloc(len + 1);
    if (dup) {
        memcpy(dup, s, len);
        dup[len] = '\0';
    }
    return dup;
}

bool pxcf_object_set(PxcfValue* object, const char* key, PxcfValue* value) {
    if (!object || object->type != PXCF_VALUE_OBJECT || !key || !value) return false;

    // Check if exists
    for (size_t i = 0; i < object->as.object.size; i++) {
        if (strcmp(object->as.object.entries[i].key, key) == 0) {
            pxcf_value_free(object->as.object.entries[i].value);
            object->as.object.entries[i].value = value;
            return true;
        }
    }

    // Append new
    if (object->as.object.capacity < object->as.object.size + 1) {
        size_t new_cap = object->as.object.capacity == 0 ? 8 : object->as.object.capacity * 2;
        PxcfObjectEntry* new_entries = (PxcfObjectEntry*)realloc(object->as.object.entries, new_cap * sizeof(PxcfObjectEntry));
        if (!new_entries) return false;
        object->as.object.entries = new_entries;
        object->as.object.capacity = new_cap;
    }

    char* dup_key = duplicate_string(key);
    if (!dup_key) return false;

    object->as.object.entries[object->as.object.size].key = dup_key;
    object->as.object.entries[object->as.object.size].value = value;
    object->as.object.size++;

    return true;
}
