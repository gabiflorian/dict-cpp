# Changelog

All notable changes to dict-cpp will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-01-18

### Added
- Initial public release
- Core dictionary implementation with integer and string keys
- Generic array support (int, double, string, bool, mixed types)
- Move semantics for zero-copy operations
- Full JSON parsing support
- JSON serialization with compact and pretty-print modes
- Type safety with runtime type checking
- Comprehensive unit tests (29 tests)
- Performance benchmarks
- Examples and documentation
- CMake and Makefile build systems
- MIT License

### Features
- 🔑 Native integer keys alongside string keys
- 🚀 C++11 move semantics
- 📦 Zero external dependencies
- 🎯 Generic arrays with mixed types
- ✨ Full JSON support (parse & serialize)
- 🛡️ Runtime type checking
- ⚡ Optimized serialization with string buffers
- 📝 Pretty-printing support

### Performance
- Move operations: ~instant (0 μs)
- Serialization: ~0.17ms per 1k items
- JSON parsing: ~9.66ms per 1000 parses
- Hash-based O(1) lookups

[1.0.0]: https://github.com/yourusername/dict-cpp/releases/tag/v1.0.0

