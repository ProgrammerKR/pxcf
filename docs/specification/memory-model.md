# PXCF Memory & Security Model

## Rationale
Configuration files often originate from untrusted sources. A malicious payload can exploit parsing limits to crash the host application via stack exhaustion (OOM), infinite loops, or integer overflow.

PXCF implementations must enforce the following security invariants:

## 1. Stack Exhaustion (Nesting Limit)
Recursive descent parsers are vulnerable to pathological inputs (e.g., `[[[[[[[[[[...`).
- Implementations **MUST** track parsing depth.
- The default maximum depth is `128` nested structures (arrays/objects).
- Exceeding the limit must safely abort parsing without leaking memory.

## 2. Integer Overflow Protection
- `Int64` bounds checking must be strict.
- Implementations must check `errno == ERANGE` (or language equivalent) during string-to-number conversion. Overflows are fatal parse errors.

## 3. Allocation Boundaries
- When loading documents from disk or network, the host application or implementation should prevent massive file buffers. (e.g., limit default file reading to 20MB unless configured otherwise).

## 4. Opaque Memory Ownership
- In native environments (C/C++, Rust FFI), the `PxcfDocument` owns the entire value tree.
- Consumers must not individually free child values retrieved from the tree.
- Deallocating the document recursively frees all associated keys, strings, and nodes.
