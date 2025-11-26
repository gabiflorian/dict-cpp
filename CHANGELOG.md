# Changelog

All notable changes to dict-cpp will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.1] - 2025-11-26

### 🔄 Header-Only Conversion

`dict-cpp` is now a single-header library! This makes integration into your projects even easier.

### Changed
- **Header-Only**: Moved all implementation from `src/dict.cpp` to `include/dict/dict.h`.
- **CMake**: Updated `dict-cpp` target to be an `INTERFACE` library.
- **Installation**: Simplified installation - just copy `dict.h`!

### Removed
- `src/dict.cpp`: Removed source file as it is no longer needed.

## [1.0.0] - 2025-11-19

### 🎉 Initial Release

First stable release of dict-cpp - Python-style dictionaries for C++!

### Added
- Core dictionary implementation with integer and string keys
- JSON parsing and serialization with pretty-printing support
- Generic arrays supporting int, double, string, bool, and mixed types
- Move semantics for zero-copy operations
- Type checking methods (`is_int()`, `is_string()`, etc.)
- Nested dictionary support with unlimited depth
- CMake and Makefile build systems
- Comprehensive unit tests (29 tests)
- Performance benchmarks vs nlohmann/json
- Multi-architecture testing (x86_64 and s390x)
- GitHub Actions CI/CD workflow
- Complete documentation and examples

### Features
- **Integer keys**: Unique feature allowing numeric dictionary keys
- **Zero dependencies**: No external libraries required
- **Cross-platform**: Linux, macOS, Windows support
- **Endian-safe**: Verified on both little-endian and big-endian systems
- **High performance**: 2-3.6x faster than nlohmann/json in key operations

### Performance Highlights
- 3.59x faster random access
- 2.89x faster integer key insertions
- 2.83x faster JSON parsing
- 2.33x faster serialization
- 1.70x faster simple insertions

### Documentation
- README with comprehensive examples
- Quick start guide
- Multi-architecture testing guide
- Contributing guidelines
- API reference
- Example programs

### Testing
- 29 unit tests with 100% pass rate
- Automated multi-architecture CI/CD
- Docker-based testing infrastructure
- Benchmarking suite

[1.0.0]: https://github.com/gabiflorian/dict-cpp/releases/tag/v1.0.0
