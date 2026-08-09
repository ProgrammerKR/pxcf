#include <stdint.h>
#include <stddef.h>
#include "pxcf/pxcf.h"

// libFuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    PxcfError err;
    PxcfDocument* doc = pxcf_parse_string((const char*)data, size, &err);
    if (doc) {
        pxcf_document_free(doc);
    }
    return 0; // Always return 0, libFuzzer treats non-zero as a failure
}
