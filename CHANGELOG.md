# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-08-09
### Added
- Initial standalone C11 implementation of PXCF.
- Hand-written UTF-8 lexer and recursive-descent parser.
- Strong memory management subsystem guaranteeing zero leaks.
- Serializer API for pretty-printing in-memory `PxcfValue` structures.
- Standalone CLI `pxcf` tool (features: `check`, `format`, `inspect`).
- Protection against duplicate keys and deep-nesting stack exhaustion.
- Comprehensive C API for embedding in other applications.
- Unit testing framework and GitHub Actions CI matrix.
