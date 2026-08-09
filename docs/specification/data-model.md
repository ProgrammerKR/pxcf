# PXCF Data Model & Types

## Type System
PXCF supports 7 primitive data types. Conversions between types are strictly prohibited in the parser. 
- `port: 8080` must be parsed as an `Int64`. It must not be implicitly coerced to `"8080"` (String) or `8080.0` (Float).
- `debug: true` must be parsed as a `Boolean`.

### 1. Null
- Represented by the literal `null`.
- Case-sensitive.

### 2. Boolean
- Represented by literals `true` and `false`.
- Case-sensitive.

### 3. Integer
- Signed 64-bit integers.
- Minimum: `-9223372036854775808`
- Maximum: `9223372036854775807`
- Must result in an overflow error if bounds are exceeded.

### 4. Float
- IEEE 754 Double Precision (64-bit) floating point number.
- Exponent notation is supported (`1e6`, `-3.14e-2`).
- Missing fractional parts (`1.`) or leading dots (`.5`) are forbidden; they must be explicit (`1.0`, `0.5`).

### 5. String
- Defined by enclosing characters in double quotes (`"`).
- Encoding is strictly UTF-8. 
- Supports escapes: `\"`, `\\`, `\/`, `\b`, `\f`, `\n`, `\r`, `\t`.
- Unicode points `\uXXXX` are supported.
- Unescaped control characters (U+0000 to U+001F) inside string literals are forbidden.

### 6. Array
- Ordered sequences of any `Value`.
- Elements do not need to be of the same type.
- Example: `[1, "two", true]`

### 7. Object
- Unordered key-value pairs.
- Keys must be Strings or unquoted identifiers.
- Keys must be strictly unique. Parsing `a: 1` and `a: 2` in the same object context must halt parsing and throw a Duplicate Key Error.
