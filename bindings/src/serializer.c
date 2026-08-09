#include "pxcf/pxcf.h"
#include "internal/value_internal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct {
    char* chars;
    size_t length;
    size_t capacity;
    bool has_error;
} StringBuilder;

static void sb_init(StringBuilder* sb) {
    sb->length = 0;
    sb->capacity = 256;
    sb->chars = (char*)malloc(sb->capacity);
    if (sb->chars) {
        sb->chars[0] = '\0';
    }
    sb->has_error = (sb->chars == NULL);
}

static void sb_append(StringBuilder* sb, const char* str) {
    if (sb->has_error) return;
    
    size_t len = strlen(str);
    if (sb->length + len + 1 > sb->capacity) {
        size_t new_cap = sb->capacity * 2;
        while (sb->length + len + 1 > new_cap) {
            new_cap *= 2;
        }
        char* new_chars = (char*)realloc(sb->chars, new_cap);
        if (!new_chars) {
            sb->has_error = true;
            return;
        }
        sb->chars = new_chars;
        sb->capacity = new_cap;
    }
    
    memcpy(sb->chars + sb->length, str, len);
    sb->length += len;
    sb->chars[sb->length] = '\0';
}

static void sb_append_char(StringBuilder* sb, char c) {
    char buf[2] = {c, '\0'};
    sb_append(sb, buf);
}

static void sb_free(StringBuilder* sb) {
    free(sb->chars);
    sb->chars = NULL;
    sb->length = 0;
    sb->capacity = 0;
}

static void serialize_value(StringBuilder* sb, const PxcfValue* value, size_t indent_level);

static void append_indent(StringBuilder* sb, size_t indent_level) {
    for (size_t i = 0; i < indent_level; i++) {
        sb_append(sb, "    ");
    }
}

static void serialize_string(StringBuilder* sb, const char* str) {
    sb_append_char(sb, '"');
    while (*str) {
        switch (*str) {
            case '"': sb_append(sb, "\\\""); break;
            case '\\': sb_append(sb, "\\\\"); break;
            case '\n': sb_append(sb, "\\n"); break;
            case '\r': sb_append(sb, "\\r"); break;
            case '\t': sb_append(sb, "\\t"); break;
            default: sb_append_char(sb, *str); break;
        }
        str++;
    }
    sb_append_char(sb, '"');
}

static bool is_valid_identifier(const char* str) {
    if (!str || !*str) return false;
    if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_')) {
        return false;
    }
    for (size_t i = 1; str[i] != '\0'; i++) {
        char c = str[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-')) {
            return false;
        }
    }
    return true;
}

static void serialize_object(StringBuilder* sb, const PxcfValue* object, size_t indent_level) {
    if (object->as.object.size == 0) {
        sb_append(sb, "{}");
        return;
    }
    
    sb_append(sb, "{\n");
    for (size_t i = 0; i < object->as.object.size; i++) {
        append_indent(sb, indent_level + 1);
        
        const char* key = object->as.object.entries[i].key;
        if (is_valid_identifier(key)) {
            sb_append(sb, key);
        } else {
            serialize_string(sb, key);
        }
        
        PxcfValue* val = object->as.object.entries[i].value;
        if (val && val->type == PXCF_VALUE_OBJECT) {
            sb_append(sb, " ");
            serialize_value(sb, val, indent_level + 1);
        } else {
            sb_append(sb, ": ");
            serialize_value(sb, val, indent_level + 1);
        }
        sb_append(sb, "\n");
    }
    append_indent(sb, indent_level);
    sb_append(sb, "}");
}

static void serialize_array(StringBuilder* sb, const PxcfValue* array, size_t indent_level) {
    if (array->as.array.size == 0) {
        sb_append(sb, "[]");
        return;
    }
    
    sb_append(sb, "[\n");
    for (size_t i = 0; i < array->as.array.size; i++) {
        append_indent(sb, indent_level + 1);
        serialize_value(sb, array->as.array.elements[i], indent_level + 1);
        if (i < array->as.array.size - 1) {
            sb_append(sb, ",");
        }
        sb_append(sb, "\n");
    }
    append_indent(sb, indent_level);
    sb_append(sb, "]");
}

static void serialize_value(StringBuilder* sb, const PxcfValue* value, size_t indent_level) {
    if (!value) {
        sb_append(sb, "null");
        return;
    }
    
    switch (value->type) {
        case PXCF_VALUE_NULL:
            sb_append(sb, "null");
            break;
        case PXCF_VALUE_BOOL:
            sb_append(sb, value->as.boolean ? "true" : "false");
            break;
        case PXCF_VALUE_INTEGER: {
            char buf[64];
            snprintf(buf, sizeof(buf), "%" PRId64, value->as.integer);
            sb_append(sb, buf);
            break;
        }
        case PXCF_VALUE_FLOAT: {
            char buf[64];
            snprintf(buf, sizeof(buf), "%g", value->as.floating);
            sb_append(sb, buf);
            break;
        }
        case PXCF_VALUE_STRING:
            serialize_string(sb, value->as.string.chars);
            break;
        case PXCF_VALUE_ARRAY:
            serialize_array(sb, value, indent_level);
            break;
        case PXCF_VALUE_OBJECT:
            serialize_object(sb, value, indent_level);
            break;
    }
}

bool pxcf_serialize_string(const PxcfValue* value, char** out_str, PxcfError* error) {
    if (!out_str) return false;
    
    StringBuilder sb;
    sb_init(&sb);
    
    if (value && value->type == PXCF_VALUE_OBJECT) {
        // Root object doesn't need wrapping braces typically, but for standard format let's just write members.
        for (size_t i = 0; i < value->as.object.size; i++) {
            const char* key = value->as.object.entries[i].key;
            if (is_valid_identifier(key)) {
                sb_append(&sb, key);
            } else {
                serialize_string(&sb, key);
            }
            
            PxcfValue* val = value->as.object.entries[i].value;
            if (val && val->type == PXCF_VALUE_OBJECT) {
                sb_append(&sb, " ");
                serialize_value(&sb, val, 0);
            } else {
                sb_append(&sb, ": ");
                serialize_value(&sb, val, 0);
            }
            sb_append(&sb, "\n\n");
        }
    } else {
        serialize_value(&sb, value, 0);
    }
    
    if (sb.has_error) {
        sb_free(&sb);
        if (error) {
            pxcf_error_init(error);
            error->code = PXCF_ERROR_OUT_OF_MEMORY;
        }
        return false;
    }
    
    *out_str = sb.chars;
    return true;
}

bool pxcf_write_file(const PxcfValue* value, const char* path, PxcfError* error) {
    if (!path) return false;
    
    char* str = NULL;
    if (!pxcf_serialize_string(value, &str, error)) {
        return false;
    }
    
    FILE* file = fopen(path, "wb");
    if (!file) {
        free(str);
        if (error) {
            pxcf_error_init(error);
            error->code = PXCF_ERROR_IO;
            snprintf(error->message, sizeof(error->message), "Could not open file '%s' for writing.", path);
        }
        return false;
    }
    
    size_t len = strlen(str);
    size_t written = fwrite(str, 1, len, file);
    fclose(file);
    free(str);
    
    if (written < len) {
        if (error) {
            pxcf_error_init(error);
            error->code = PXCF_ERROR_IO;
            snprintf(error->message, sizeof(error->message), "Could not fully write to file '%s'.", path);
        }
        return false;
    }
    
    return true;
}
