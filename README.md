# dict-cpp

> Python-style dictionaries for C++ with JSON serialization

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)
[![Multi-Arch Tests](https://github.com/gabiflorian/dict-cpp/actions/workflows/multi-arch-tests.yml/badge.svg)](https://github.com/gabiflorian/dict-cpp/actions/workflows/multi-arch-tests.yml)

**dict-cpp** brings Python's flexible dictionaries to C++. Unlike other JSON libraries, dict-cpp supports **native integer keys** alongside string keys, just like Python dictionaries!

## Features

- 🔑 **Integer & String Keys** - Use both! `d[0]` and `d["key"]` work together
- 🚀 **Move Semantics** - Zero-copy operations with C++11 move semantics
- 📦 **Zero Dependencies** - Just two files, no external libraries
- 🎯 **Generic Arrays** - Support for int, double, string, bool, and mixed types
- ✨ **Full JSON Support** - Parse and serialize standard JSON
- 🛡️ **Type Safe** - Runtime type checking with `is_int()`, `is_string()`, etc.
- ⚡ **Fast Serialization** - Optimized string buffer implementation
- 📝 **Pretty Printing** - Human-readable JSON with indentation

## Quick Example

```cpp
#include "dict/dict.h"

// Python: d = {"name": "Alice", 0: "first", 42: "answer"}
dict_t d;
d["name"] = "Alice";    // string key
d[0] = "first";         // integer key  
d[42] = "answer";       // integer key

// Serialize to JSON
std::cout << d.dump(2) << std::endl;  // Pretty-printed!

// Parse JSON
dict_t parsed(R"({"name":"Bob","age":30})");
std::cout << parsed["name"].str << std::endl;  // "Bob"
```

## Why dict-cpp?

### Python-like Flexibility
```python
# Python
user = {
    "name": "Alice",
    0: "admin",      # Integer keys!
    1: "verified",
    "scores": [95, 87, 92]
}
```

```cpp
// C++ with dict-cpp - Same flexibility!
dict_t user;
user["name"] = "Alice";
user[0] = "admin";          // Integer keys work!
user[1] = "verified";
user["scores"] = vector<int>{95, 87, 92};
```

**Note:** Popular libraries like nlohmann/json don't support integer keys!

## Installation

### Option 1: Copy Files
Just copy two files into your project:
```bash
cp include/dict/dict.h your_project/
cp src/dict.cpp your_project/
```

### Option 2: CMake
```cmake
add_subdirectory(dict-cpp)
target_link_libraries(your_target dict-cpp)
```

### Option 3: Header-Only (Coming Soon)
Single-header version for maximum convenience.

## Usage

### Basic Operations

```cpp
#include "dict/dict.h"

dict_t person;
person["name"] = "Alice";
person["age"] = 25;
person["active"] = true;
person["score"] = 98.5;
```

### Generic Arrays

```cpp
// Integer arrays
person["numbers"] = vector<int>{1, 2, 3, 4, 5};

// String arrays
person["tags"] = vector<string>{"admin", "verified"};

// Mixed-type arrays (like JavaScript!)
val_t mixed;
mixed.push_back(42);
mixed.push_back("text");
mixed.push_back(3.14);
mixed.push_back(true);
person["data"] = mixed;
```

### JSON Parsing

```cpp
string json = R"({
    "user": {
        "name": "Bob",
        "scores": [95, 87, 92],
        "verified": true
    }
})";

dict_t data(json);
cout << data["user"]["name"].str << endl;  // "Bob"
```

### JSON Serialization

```cpp
// Compact format
string compact = person.dump();

// Pretty format (2-space indent)
string pretty = person.dump(2);
cout << pretty << endl;
```

### Type Checking

```cpp
if (data["age"].is_int()) {
    cout << "Age: " << data["age"].intg << endl;
}

if (data["scores"].is_array()) {
    for (size_t i = 0; i < data["scores"].size(); i++) {
        cout << data["scores"].at(i).intg << endl;
    }
}
```

### Move Semantics (Fast!)

```cpp
dict_t create_data() {
    dict_t d;
    // ... populate d ...
    return d;  // Automatically moved (fast!)
}

dict_t data = create_data();  // No copy, instant!
```

## API Reference

### Types

| Type | Check | Access |
|------|-------|--------|
| null | `is_null()` | - |
| bool | `is_bool()` | `.boolean` |
| int | `is_int()` | `.intg` |
| double | `is_double()` | `.dbl` |
| string | `is_string()` | `.str` |
| array | `is_array()` | `.at(i)`, `.size()` |
| object | `is_object()` | `["key"]` |

### Dictionary Operations

```cpp
dict_t d;

// Set values
d["key"] = value;           // string key
d[integer] = value;          // integer key

// Get values
string s = d["key"].str;
int i = d["key"].intg;

// Nested access
d["user"]["name"] = "Alice";

// JSON operations
d.parse(json_string);        // Parse JSON
string json = d.dump();      // Serialize (compact)
string pretty = d.dump(2);   // Serialize (pretty)
```

### Array Operations

```cpp
dict_t d;
d["list"] = vector<int>{1, 2, 3};

// Array methods
size_t len = d["list"].size();
val_t first = d["list"].at(0);
d["list"].push_back(4);
```

## Performance

Benchmarked against nlohmann/json on:
- **CPU:** Intel Xeon E5-1650 v4 @ 3.60GHz (12 cores)
- **RAM:** 64GB
- **OS:** Ubuntu 22.04.5 LTS
- **Compiler:** g++ 11.4.0 with -O2 optimization

### Benchmark Results

| Operation | nlohmann | dict-cpp | Speedup |
|-----------|----------|----------|---------|
| Simple insertions (10k) | 10.73 ms | 6.31 ms | **1.70x faster** ⚡ |
| Integer key insertions (10k) | 10.12 ms | 3.50 ms | **2.89x faster** ⚡ |
| Nested objects (1k) | 3.95 ms | 3.64 ms | **1.09x faster** |
| Serialization (1k) | 0.37 ms | 0.16 ms | **2.33x faster** ⚡ |
| JSON parsing (1000x) | 25.19 ms | 8.91 ms | **2.83x faster** ⚡ |
| Random access (5k) | 3.98 ms | 1.11 ms | **3.59x faster** 🚀 |
| Copy operations (100x500) | 18.82 ms | 31.01 ms | 0.61x (slower) |

**Key takeaways:**
- ⚡ **Up to 3.6x faster** for random access operations
- 🚀 **2-3x faster** for parsing, serialization, and insertions
- 💪 Competitive performance in most operations
- 📊 Only slower in deep copy operations (nlohmann is highly optimized here)

For detailed benchmarks, see [benchmarks/](benchmarks/).

## Building

### With CMake
```bash
mkdir build && cd build
cmake ..
make
```

### With Make
```bash
make
make test
```

### Manual
```bash
g++ -std=c++11 -Wall -Wextra your_program.cpp src/dict.cpp -Iinclude -o your_program
```

## Examples

See [examples/](examples/) directory for complete examples:
- [basic.cpp](examples/basic.cpp) - Basic usage
- [parsing.cpp](examples/parsing.cpp) - JSON parsing
- [arrays.cpp](examples/arrays.cpp) - Working with arrays
- [api_response.cpp](examples/api_response.cpp) - Real-world API example

## Testing

```bash
# Run unit tests
make test

# Run benchmarks
make benchmark
```

## Comparison with nlohmann/json

| Feature | dict-cpp | nlohmann/json |
|---------|----------|---------------|
| Integer keys | ✅ Yes | ❌ No |
| String keys | ✅ Yes | ✅ Yes |
| Move semantics | ✅ Yes | ✅ Yes |
| JSON parsing | ✅ Yes | ✅ Yes |
| Dependencies | ✅ Zero | ✅ Zero (header-only) |
| Size | ~850 lines | ~25k lines |
| Simplicity | ✅ Easy to understand | Complex templates |
| Performance | Competitive | Highly optimized |

**When to use dict-cpp:**
- ✅ You need integer keys (unique feature!)
- ✅ You want simple, readable code
- ✅ Educational projects
- ✅ You need to understand/modify internals
- ✅ Embedded systems (small footprint)

**When to use nlohmann:**
- You need maximum performance everywhere
- You need advanced features (JSON Pointer, Patch, etc.)
- Large team projects (everyone knows it)

## Requirements

- C++11 or later
- No external dependencies

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

MIT License - see [LICENSE](LICENSE) for details.

## Acknowledgments

Inspired by Python's flexible dictionary implementation and nlohmann/json's excellent API design.

## Support

- 📖 [Documentation](docs/)
- 💬 [Issues](../../issues)
- 🌟 If you find this useful, please star the repo!

---

**dict-cpp** - Bring Python's flexibility to C++! 🐍➕➕

