# PXCF Specification

## 1. Introduction
PXCF is a lightweight, human-readable configuration format designed for predictable parsing and structured data representation.

## 2. Document Structure
A PXCF document consists of a sequence of members (key-value pairs) or object blocks.

## 3. Data Types
- **Null:** `null`
- **Boolean:** `true` or `false`
- **Integer:** 64-bit signed integer (e.g., `42`, `-7`)
- **Float:** 64-bit floating point (e.g., `3.14`, `-1e6`)
- **String:** Double-quoted UTF-8 strings with standard escapes (`\n`, `\"`, `\\`, etc.)
- **Array:** Ordered list of values enclosed in `[` and `]` separated by commas.
- **Object:** Key-value pairs enclosed in `{` and `}`.

## 4. Keys and Properties
Keys can be unquoted identifiers or quoted strings.
Properties are defined as `key: value`.

If the value is an object, the colon is optional:
```
server {
    port: 8080
}
```

## 5. Comments
Single-line comments begin with `#` and extend to the end of the line.
