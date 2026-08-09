# Security Policy

## Supported Versions

Currently, the following versions of PXCF are supported with security updates.

| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |

## Reporting a Vulnerability

Security is a core design principle for PXCF, specifically regarding untrusted input, pathological parsing limits (stack exhaustion), and memory safety.

If you discover a potential vulnerability in PXCF, please **do not** disclose it publicly.

Instead, please send an email to the security team at **security@proxentix.com**. We will respond as quickly as possible.

### Expected Vulnerabilities
Issues considered critical:
- Buffer overflows, Out-of-Bounds (OOB) reads/writes.
- Parsing a maliciously crafted file causing memory leaks or Use-After-Free (UAF) execution.
- Undefined behavior that allows arbitrary code execution or crashes the embedding host application.
