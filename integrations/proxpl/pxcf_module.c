#include "pxcf/pxcf.h"

// Pseudo C++ module file illustrating how ProXPL integrates with PXCF
/*
#include "proxpl/runtime.h"
#include "proxpl/object.h"

ProxValue prox_pxcf_load(ProxVM* vm, int argCount, ProxValue* args) {
    if (argCount != 1 || !IS_STRING(args[0])) {
        // Handle err
    }
    const char* path = AS_CSTRING(args[0]);
    PxcfDocument* doc = pxcf_load_file(path, NULL);
    if (!doc) {
        return PROX_NULL_VAL;
    }
    
    // Map the document elements natively into ProX objects using the pxcf_object_get_index C API
    ProxObject* prox_obj = new_prox_object(vm);
    // ... loop over doc properties ...
    
    pxcf_document_free(doc);
    return OBJ_VAL(prox_obj);
}

void prox_register_pxcf(ProxVM* vm) {
    prox_define_native(vm, "pxcf_load", prox_pxcf_load);
}
*/
