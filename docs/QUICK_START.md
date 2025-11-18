# Quick Start Guide

## Installation

### Option 1: Copy Files
```bash
# Copy just two files into your project
cp include/dict/dict.h your_project/
cp src/dict.cpp your_project/
```

### Option 2: CMake
```cmake
add_subdirectory(dict-cpp)
target_link_libraries(your_target dict-cpp)
```

### Option 3: Manual Compilation
```bash
g++ -std=c++11 your_program.cpp dict-cpp/src/dict.cpp -Idict-cpp/include -o your_program
```

## Basic Usage

```cpp
#include "dict/dict.h"

int main() {
    // Create dictionary
    dict_t person;
    person["name"] = "Alice";
    person["age"] = 25;
    person[0] = "admin";        // Integer key!
    
    // Serialize to JSON
    std::cout << person.dump(2) << std::endl;
    
    return 0;
}
```

## Key Features in 30 Seconds

### 1. Integer & String Keys
```cpp
dict_t d;
d["key"] = "string key";
d[42] = "integer key";
```

### 2. Generic Arrays
```cpp
d["integers"] = vector<int>{1, 2, 3};
d["strings"] = vector<string>{"a", "b"};
```

### 3. JSON Parsing
```cpp
dict_t parsed(R"({"name":"Bob","age":30})");
cout << parsed["name"].str << endl;  // "Bob"
```

### 4. Type Checking
```cpp
if (d["age"].is_int()) {
    cout << d["age"].intg << endl;
}
```

### 5. Move Semantics
```cpp
dict_t moved = std::move(create_dict());  // Fast!
```

## Next Steps

- See [examples/](../examples/) for complete code
- Read [API Reference](API.md) for full documentation
- Check [README.md](../README.md) for detailed features

## Common Patterns

### Building Complex Structures
```cpp
dict_t api;
api["status"] = "success";
api["code"] = 200;

dict_t user;
user["id"] = 1;
user["name"] = "Alice";

api["user"] = user;
```

### Working with Arrays
```cpp
dict_t d;
d["scores"] = vector<int>{95, 87, 92};

for (size_t i = 0; i < d["scores"].size(); i++) {
    cout << d["scores"].at(i).intg << endl;
}
```

### Parsing API Responses
```cpp
string json = fetch_from_api();
dict_t response(json);

if (response["status"].str == "success") {
    process(response["data"]);
}
```

That's it! You're ready to use dict-cpp! 🚀

