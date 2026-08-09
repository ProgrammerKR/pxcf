# PXCF Lexical Structure

## Source Encoding
A canonical PXCF document MUST be encoded in valid UTF-8. 
Parsers encountering invalid UTF-8 byte sequences must abort and report a fatal error.

## Line Endings
Parsers must uniformly accept both `\n` (LF) and `\r\n` (CRLF) as valid line terminators for line-tracking error bounds.

## Whitespace
Whitespace is defined as space (U+0020), tab (U+0009), line feed (U+000A), and carriage return (U+000D). Whitespace is allowed anywhere except within tokens (unless explicitly part of a string).

## Comments
Comments begin with a hash character `#` and run to the end of the line. They are ignored by the parser and are treated semantically identical to whitespace.
