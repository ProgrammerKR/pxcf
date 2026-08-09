#include <stdint.h>
#include <stddef.h>
#include "pxcf/pxcf.h"

// Fuzzing entrypoint for libFuzzer
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    PxcfError err;
    PxcfDocument* doc = pxcf_parse_string((const char*)data, size, &err);
    if (doc) {
        // If parsed, try to serialize to ensure symmetric memory safety
        char* out = NULL;
        pxcf_serialize_string(pxcf_document_root(doc), &out, NULL);
        if (out) free(out);
        pxcf_document_free(doc);
    }
    return 0; // Values other than 0 and -1 are reserved for future use.
}
