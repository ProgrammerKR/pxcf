# PXCF — ProX Configuration File

PXCF is a robust, production-quality, and completely standalone configuration format and parsing ecosystem written in pure C11. 

Designed originally for the **ProXPL** language ecosystem, PXCF has evolved into a fully independent, language-agnostic data serialization format. It marries the robust, predictable structure of JSON with the clean, human-friendly readability of TOML and YAML, without inheriting their respective complexities and edge-case ambiguities.

---

## 🌟 Philosophy & Goals

1. **Simple to learn:** The syntax is intuitive and lacks excessive punctuation.
2. **Difficult to misuse:** Strongly typed with no implicit conversions or magic parsing.
3. **Strict when invalid:** The parser refuses to "guess" on malformed input.
4. **Predictable & Deterministic:** Consistent in-memory representations and round-trip serializations.
5. **Secure & Robust:** Memory boundaries, maximum nesting depth, and allocation strategies are built with untrusted input in mind.
6. **Zero Dependencies:** Written in standard C11. No external libraries are required to embed the core.

---

## 🛠️ Key Features

- **Human-Readable Syntax:** Nested structures without requiring quotes on identifiers or colons on object blocks.
- **Strong Typing:** Native representation of `Null`, `Booleans`, `64-bit Integers`, `64-bit Floats`, `Strings`, `Arrays`, and `Objects`.
- **Comments:** Native support for `#` single-line comments.
- **Duplicate Key Protection:** The parser explicitly flags and rejects duplicate keys to prevent configuration shadow-overwrites.
- **Robust Error Reporting:** Comprehensive error payloads featuring `line`, `column`, `byte offset`, and exact failure messages.
- **Zero GC:** Deterministic, manual memory management tailored for easy integration into language runtimes, game engines, and embedded software.
- **Command-Line Interface:** Ships with `pxcf`, a utility for validating, inspecting, and auto-formatting configuration files.

---

## 📝 Example Configuration

```pxcf
# ProXPL Project Configuration

project {
    name: "MyProXProject"
    version: "1.0.0"
    language: "proxpl"
    entry: "src/main.prox"
}

build {
    mode: "release"
    optimization: true
    debug: false
    target: "native"
    
    # Array configuration
    targets: [
        "windows",
        "linux",
        "macos"
    ]
}

features {
    jit: true
    gc: true
    warnings: true
}

dependencies {
    http: "1.2.0"
    json: "2.0.1"
}
```

---

## 🚀 Building from Source

### Prerequisites
- CMake 3.10+
- A C11 compatible compiler (GCC, Clang, MSVC)

### Standard Build

Clone the repository and build using standard CMake commands:

```bash
git clone https://github.com/ProXentix/PXCF.git
cd PXCF
cmake -B build
cmake --build build --config Release
```

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `PXCF_BUILD_CLI` | `ON` | Build the standalone `pxcf` executable |
| `PXCF_BUILD_TESTS` | `ON` | Build the unit and conformance test suites |
| `PXCF_BUILD_SHARED` | `OFF` | Build PXCF as a dynamic shared library (`.so`/`.dll`/`.dylib`) |
| `PXCF_ENABLE_SANITIZERS`| `OFF` | Enable AddressSanitizer and UndefinedBehaviorSanitizer (Clang/GCC) |

---

## 💻 Command-Line Tool

The project includes `pxcf`, a powerful CLI tool for interacting with PXCF documents.

### Validating a File
Checks a file for syntax errors and structural integrity without modifying it.
```bash
pxcf check config.pxcf
```

### Formatting a File
Pretty-prints the file to standard output. 
```bash
pxcf format config.pxcf
```
To overwrite the file in-place with the formatted output:
```bash
pxcf format config.pxcf --write
```

### Inspecting a File
Parses the file and outputs the structural in-memory node graph.
```bash
pxcf inspect config.pxcf
```

---

## 🧩 C API Usage

Integrating PXCF into your own C/C++ project is designed to be trivial. The API strictly avoids exposing internal structures, relying on opaque pointers for safe usage.

### Parsing a string

```c
#include <pxcf/pxcf.h>
#include <stdio.h>

int main() {
    const char* source = "server { port: 8080 }";
    
    PxcfError err;
    PxcfDocument* doc = pxcf_parse_string(source, 0, &err);
    
    if (!doc) {
        printf("Parse Error on Line %zu: %s\n", err.line, err.message);
        return 1;
    }

    PxcfValue* root = pxcf_document_root(doc);
    PxcfValue* server = pxcf_object_get(root, "server");
    PxcfValue* port = pxcf_object_get(server, "port");

    int64_t port_val = 0;
    pxcf_value_get_integer(port, &port_val);
    
    printf("Server Port: %lld\n", (long long)port_val);

    // Free the entire document and value tree at once
    pxcf_document_free(doc);
    return 0;
}
```

---

## 🛡️ Security & Thread Safety

- **Thread Safety:** The library has zero global mutable state. Documents parsed in separate threads are completely thread-safe. A single `PxcfDocument` should not be mutated concurrently without external synchronization.
- **Nesting Limits:** To prevent stack exhaustion and crash vulnerabilities via pathological inputs, recursive structures (Nested Objects/Arrays) are capped.
- **Sanitizers:** The codebase is rigorously tested under ASAN/UBSAN to guarantee the absence of memory leaks, double-frees, or UB.

---

## 🌍 Language Ecosystem & Publishing

Since PXCF 2.1 is designed as a standalone ecosystem, the language bindings can be packaged and published using their native toolchains targeting the single hardened C11 engine.

### Python (PyPI)
The Python C-Extension natively bridges the parser into Python dictionaries.
```bash
cd bindings/python
python -m pip install .
python -m build
python -m twine upload dist/*
```

### JavaScript / TypeScript (npm)
The JavaScript binding compiles the C11 core to WebAssembly via Emscripten. Requires `emcc` and `Node.js` installed.
```bash
cd bindings/javascript
npm run build
npm publish
```

### Rust (crates.io)
The safe Rust idiomatic crate binds the native parser using the `cc` build system via FFI.
```bash
cd bindings/rust
cargo build --release
cargo publish
```

### Go
Go handles packaging directly from the VCS repository using `cgo`.
```bash
go get github.com/ProgrammerKR/pxcf/bindings/go
```

### ProXPL
Integrations into the ProX programming language runtime are structurally prepared inside `integrations/proxpl/`.

---

## 🤝 Contributing

We welcome contributions!
If you wish to submit bug fixes, optimize performance, or add language bindings, please feel free to open a Pull Request.
Ensure that:
- Your code conforms strictly to C11.
- You do not introduce any third-party dependencies into the core engine.
- All tests and sanitizers pass locally via `ctest`.

## 📜 License

This project is part of the **ProXentix** ecosystem and is open-sourced under the MIT License. See the `LICENSE` file for details.
