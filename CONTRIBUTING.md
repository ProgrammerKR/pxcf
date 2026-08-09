# Contributing to PXCF

First off, thank you for considering contributing to PXCF! It's people like you that make PXCF such a great ecosystem.

## How to Contribute

### 1. Reporting Bugs
- Ensure the bug was not already reported by searching on GitHub under Issues.
- Open a new Issue and provide a clear and descriptive title.
- Include a minimal `.pxcf` file that reproduces the issue if applicable.

### 2. Suggesting Enhancements
- Provide a clear and descriptive title for the issue.
- Describe the exact enhancement or new grammar rule. Keep in mind that PXCF intentionally maintains a small specification. Feature creep is heavily discouraged.

### 3. Submitting Pull Requests
- Keep your code clean, standard C11 compliant, and memory-leak free.
- Run `cmake --build build --config Debug` and ensure you test with ASAN enabled.
- Ensure all existing unit tests in `tests/unit` pass.
- Write new tests for your fixes or enhancements.
- Update documentation if applicable.
- Make sure your PR commits are atomic and descriptive.

## Code Style
The codebase follows standard modern C guidelines.
- Use `size_t` for lengths and capacities.
- Avoid hidden magic numbers; use `#define` or `enum`.
- Avoid naked `malloc` without ensuring proper ownership transfer or cleanup.

Thank you!
