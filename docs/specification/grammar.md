# PXCF Formal Grammar

PXCF is designed to be deterministic and context-free, making it simple to parse using recursive descent.

## EBNF Grammar

```ebnf
document        ::= (property_list)?

property_list   ::= property (property)*
property        ::= identifier (":" value)? | identifier "{" property_list? "}"

identifier      ::= [a-zA-Z_] [a-zA-Z0-9_]* | string

value           ::= string | integer | float | boolean | null | array | object

string          ::= '"' (character | escape_sequence)* '"'
integer         ::= "-"? [0-9]+
float           ::= "-"? [0-9]+ "." [0-9]+ ([eE] ["+" "-"]? [0-9]+)?
boolean         ::= "true" | "false"
null            ::= "null"

array           ::= "[" (value ("," value)*)? "]"
object          ::= "{" (property)* "}"

escape_sequence ::= "\" ["\\/bfnrt] | "\u" [0-9a-fA-F]{4}
```

## Structure Rules
1. A PXCF document is conceptually an implicit `Object`.
2. Property definitions inside an object may omit the colon `:` if the assigned value is an inline object block `identifier { ... }`.
3. Arrays are comma-separated `[1, 2, 3]`.
4. Objects are a whitespace-separated sequence of properties.
5. Keys must be unique within an object. Duplicate keys are invalid.
