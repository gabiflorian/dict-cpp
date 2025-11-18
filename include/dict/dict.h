/*
 * MIT License
 * 
 * Copyright (c) 2025 Gavril Ioan Florian <gv.florian@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef DICT_CPP_H
#define DICT_CPP_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include <sstream>
#include <cctype>

// Forward declarations
struct dict_s;
struct val_s;

typedef enum {
    json_object_t,
    json_string_t,
    json_integer_t,
    json_double_t,
    json_array,
    json_bool_t,
    json_null_t
} json_value_types;

typedef enum {
    json_k_integer_t,
    json_k_string_t,
} json_key_types;

// Value structure with generic array support
struct val_s {
    json_value_types t;
    std::shared_ptr<dict_s> obj;
    std::string str;
    int64_t intg;
    double dbl;
    bool boolean;
    
    // Generic array - can hold any value type
    std::vector<std::shared_ptr<val_s>> arr;
    
    // Constructors
    val_s();
    val_s(const val_s& other);           // Copy constructor
    val_s(val_s&& other) noexcept;       // Move constructor
    
    // Assignment operators
    val_s& operator=(const val_s& other);       // Copy assignment
    val_s& operator=(val_s&& other) noexcept;   // Move assignment
    
    // Type-specific assignments
    val_s& operator=(int64_t assign);
    val_s& operator=(int assign);  // For convenience
    val_s& operator=(const std::string& assign);
    val_s& operator=(const char* assign);  // For convenience
    val_s& operator=(double assign);
    val_s& operator=(bool assign);
    val_s& operator=(const dict_s& assign);
    
    // Generic array assignment
    template<typename T>
    val_s& operator=(const std::vector<T>& assign);
    
    // Subscript operators
    val_s& operator[](const std::string& key);
    val_s& operator[](int32_t i);
    val_s& operator[](size_t i);
    
    // Array access
    val_s& at(size_t index);
    const val_s& at(size_t index) const;
    size_t size() const;
    void push_back(const val_s& val);
    template<typename T>
    void push_back(const T& val);
    
    // Type checking
    bool is_null() const { return t == json_null_t; }
    bool is_bool() const { return t == json_bool_t; }
    bool is_int() const { return t == json_integer_t; }
    bool is_double() const { return t == json_double_t; }
    bool is_string() const { return t == json_string_t; }
    bool is_array() const { return t == json_array; }
    bool is_object() const { return t == json_object_t; }
    
    // Destructor
    ~val_s();
};

// Key structure
struct key_s {
    json_key_types t;
    std::string str;
    int32_t intg;
    
    key_s() : t(json_k_string_t), intg(0) {}
};

// Hash function for keys
struct json_key_hash {
    std::size_t operator()(const key_s& k) const;
};

// Dictionary structure
struct dict_s {
#ifdef JSON_ORDERED
    typedef std::map<key_s, val_s> map_t;
#else
    typedef std::unordered_map<key_s, val_s, json_key_hash> map_t;
#endif
    
    std::shared_ptr<map_t> obj;
    bool valid;
    
    // Constructors
    dict_s();
    dict_s(const std::string& json_str);    // JSON parsing constructor
    dict_s(const dict_s& other);             // Copy constructor
    dict_s(dict_s&& other) noexcept;         // Move constructor
    
    // Assignment operators
    dict_s& operator=(const dict_s& other);        // Copy assignment
    dict_s& operator=(dict_s&& other) noexcept;    // Move assignment
    
    // Subscript operators
    val_s& operator[](const std::string& s);
    val_s& operator[](int32_t i);
    
    // JSON parsing
    bool parse(const std::string& json_str);
    
    // Optimized serialization
    std::string dump(int indent = -1) const;
    void dump_internal(std::ostringstream& oss, int indent, int current_level) const;
    
    // Destructor
    ~dict_s();
};

typedef dict_s dict_t;
typedef val_s val_t;
typedef key_s dict_key_t;

// Comparison operators
#ifndef JSON_ORDERED
bool operator==(const key_s& first, const key_s& second);
#else
bool operator<(const key_s& first, const key_s& second);
#endif

// Stream operators
std::ostream& operator<<(std::ostream& os, const dict_t& dict);
std::ostream& operator<<(std::ostream& os, const val_t& val);
std::ostream& operator<<(std::ostream& os, const dict_key_t& k);

// Template implementations for generic arrays

template<typename T>
val_s& val_s::operator=(const std::vector<T>& assign) {
    t = json_array;
    arr.clear();
    for (const auto& item : assign) {
        auto v = std::make_shared<val_s>();
        *v = item;
        arr.push_back(v);
    }
    return *this;
}

template<typename T>
void val_s::push_back(const T& val) {
    if (t != json_array) {
        t = json_array;
        arr.clear();
    }
    auto v = std::make_shared<val_s>();
    *v = val;
    arr.push_back(v);
}

// JSON Parser class
class JSONParser {
private:
    const char* cursor;
    const char* end;
    
    void skip_whitespace();
    bool parse_value(val_s& val);
    bool parse_object(dict_s& dict);
    bool parse_array(val_s& val);
    bool parse_string(std::string& str);
    bool parse_number(val_s& val);
    bool parse_literal(val_s& val);
    
public:
    JSONParser(const std::string& json);
    bool parse(dict_s& dict);
    bool parse(val_s& val);
};

#endif // DICT_CPP_H

