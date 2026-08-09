# PXCF Serialization & Formatting Rules

The PXCF format requires deterministic and unambiguous serialization to ensure parity across languages and platforms. 

## Whitespace and Indentation
- The default indentation for formatted output is **4 spaces**.
- Tabs must not be used for indentation during serialization.

## Strings and Keys
- Object keys should be output as bare identifiers if they match the regex `^[a-zA-Z_][a-zA-Z0-9_]*$`.
- If an object key contains spaces, special characters, or starts with a number, it **MUST** be double-quoted.
- All String values must be double quoted.

## Array Formatting
- If an array is completely empty, it should be serialized as `[]`.
- If an array contains values, each value should be serialized on a new line, indented one level deeper than the array key.
- A trailing comma is optional in PXCF syntax but should NOT be emitted by the canonical serializer.

## Object Formatting
- `identifier { ... }` is the preferred inline syntax if the value of a property is a nested object. Do not emit colons before object blocks. 
- E.g., Correct: `server { port: 8080 }`. Incorrect: `server: { port: 8080 }`.

## Ordering
- For deterministic formatting, the in-memory graph should ideally preserve the exact insertion order of the parsed document, and serialize keys in that order. 
