# PXCF 3.0.0 Specification

## Lexical Grammar

PXCF is fundamentally a UTF-8 configuration language designed for human readability and deterministic parsing.

```ebnf
PXCFDocument = { Property }
Property     = Key ":" Value | Key "{" ObjectMembers "}"
Key          = Identifier | String
Identifier   = /[a-zA-Z_][a-zA-Z0-9_]*/
Value        = String | Number | Boolean | Null | Array | Object
String       = '"' { UnescapedCharacter | EscapedCharacter } '"'
Number       = Integer | Float
Integer      = ["-"] ( "0" | /[1-9][0-9]*/ )
Float        = Integer "." /[0-9]+/
Boolean      = "true" | "false"
Null         = "null"
Array        = "[" [ Value { "," Value } ] "]"
Object       = "{" ObjectMembers "}"
ObjectMembers= { Property }
```

## Security Limits
To prevent denial of service through stack overflow or memory exhaustion, PXCF 3.0 strictly defines bounds:
- **Maximum Nesting Depth:** 128 (Parsers must reject documents exceeding this limit).
- **Duplicate Keys:** Duplicate keys within the same object block result in a strict syntax error.

## Memory Ownership
When interfacing via C, `PxcfDocument` is an opaque, explicitly owned document handle. The external consumer is solely responsible for calling `pxcf_document_free` to reclaim allocations cleanly. No global state is maintained by the parser.

## Error Handling
The parser must not crash on malformed inputs. It must populate the `PxcfError` structure with a deterministic error code, the line and column number, and a human-readable message.
