#ifndef PXCF_VALUE_INTERNAL_H
#define PXCF_VALUE_INTERNAL_H

#include "pxcf/pxcf_value.h"

typedef struct PxcfObjectEntry {
    char* key;
    PxcfValue* value;
} PxcfObjectEntry;

struct PxcfValue {
    PxcfValueType type;
    union {
        bool boolean;
        int64_t integer;
        double floating;
        struct {
            char* chars;
            size_t length;
        } string;
        struct {
            PxcfValue** elements;
            size_t size;
            size_t capacity;
        } array;
        struct {
            PxcfObjectEntry* entries;
            size_t size;
            size_t capacity;
        } object;
    } as;
};

#endif // PXCF_VALUE_INTERNAL_H
