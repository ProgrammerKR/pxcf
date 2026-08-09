# ProXPL Integration Architecture

The PXCF module in ProXPL exposes the C API directly to the runtime:

```prox
import pxcf

fn main() {
    let config = pxcf.load("project.pxcf")
    print(config.project.name)
}
```

The C-bindings layer should dynamically link against `libpxcf.so` and wrap the opaque `PxcfValue` pointers with native ProXPL garbage-collected wrappers. Since PXCF manages its own tree manually, the ProXPL GC finalizer should invoke `pxcf_document_free` when the `config` object falls out of scope.
