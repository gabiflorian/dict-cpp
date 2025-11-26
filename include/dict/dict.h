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
#include <cstdlib>
#include <cstring>
#include <algorithm>

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

// ==================== val_s Implementation ====================

inline val_s::val_s() : t(json_null_t), intg(0), dbl(0.0), boolean(false) {}

inline val_s::val_s(const val_s& other) 
    : t(other.t), str(other.str), intg(other.intg), dbl(other.dbl), boolean(other.boolean) {
    if (other.obj) {
        obj = std::make_shared<dict_s>(*other.obj);
    }
    // Deep copy array
    for (const auto& item : other.arr) {
        arr.push_back(std::make_shared<val_s>(*item));
    }
}

// Move constructor - OPTIMIZATION #1: Move Semantics
inline val_s::val_s(val_s&& other) noexcept
    : t(other.t), obj(std::move(other.obj)), str(std::move(other.str)), 
      intg(other.intg), dbl(other.dbl), boolean(other.boolean), arr(std::move(other.arr)) {
    other.t = json_null_t;
}

inline val_s::~val_s() {}

// Copy assignment
inline val_s& val_s::operator=(const val_s& other) {
    if (this != &other) {
        t = other.t;
        str = other.str;
        intg = other.intg;
        dbl = other.dbl;
        boolean = other.boolean;
        
        if (other.obj) {
            obj = std::make_shared<dict_s>(*other.obj);
        } else {
            obj.reset();
        }
        
        arr.clear();
        for (const auto& item : other.arr) {
            arr.push_back(std::make_shared<val_s>(*item));
        }
    }
    return *this;
}

// Move assignment - OPTIMIZATION #1: Move Semantics
inline val_s& val_s::operator=(val_s&& other) noexcept {
    if (this != &other) {
        t = other.t;
        obj = std::move(other.obj);
        str = std::move(other.str);
        intg = other.intg;
        dbl = other.dbl;
        boolean = other.boolean;
        arr = std::move(other.arr);
        other.t = json_null_t;
    }
    return *this;
}

// Type-specific assignments
inline val_s& val_s::operator=(int64_t assign) {
    t = json_integer_t;
    intg = assign;
    return *this;
}

inline val_s& val_s::operator=(int assign) {
    return (*this) = static_cast<int64_t>(assign);
}

inline val_s& val_s::operator=(const std::string& assign) {
    t = json_string_t;
    str = assign;
    return *this;
}

inline val_s& val_s::operator=(const char* assign) {
    return (*this) = std::string(assign);
}

inline val_s& val_s::operator=(double assign) {
    t = json_double_t;
    dbl = assign;
    return *this;
}

inline val_s& val_s::operator=(bool assign) {
    t = json_bool_t;
    boolean = assign;
    return *this;
}

inline val_s& val_s::operator=(const dict_s& assign) {
    t = json_object_t;
    obj = std::make_shared<dict_s>(assign);
    return *this;
}

inline val_s& val_s::operator[](const std::string& key) {
    return (*obj)[key];
}

inline val_s& val_s::operator[](int32_t i) {
    return (*obj)[i];
}

inline val_s& val_s::operator[](size_t i) {
    if (t != json_array || i >= arr.size()) {
        throw std::out_of_range("Array index out of range");
    }
    return *arr[i];
}

// OPTIMIZATION #2: Generic Arrays - Array access methods
inline val_s& val_s::at(size_t index) {
    if (t != json_array || index >= arr.size()) {
        throw std::out_of_range("Array index out of range");
    }
    return *arr[index];
}

inline const val_s& val_s::at(size_t index) const {
    if (t != json_array || index >= arr.size()) {
        throw std::out_of_range("Array index out of range");
    }
    return *arr[index];
}

inline size_t val_s::size() const {
    if (t == json_array) {
        return arr.size();
    }
    return 0;
}

inline void val_s::push_back(const val_s& val) {
    if (t != json_array) {
        t = json_array;
        arr.clear();
    }
    arr.push_back(std::make_shared<val_s>(val));
}

// ==================== dict_s Implementation ====================

inline dict_s::dict_s() : obj(std::make_shared<map_t>()), valid(true) {}

// OPTIMIZATION #4: JSON Parsing Constructor
inline dict_s::dict_s(const std::string& json_str) : obj(std::make_shared<map_t>()), valid(false) {
    valid = parse(json_str);
}

// Copy constructor
inline dict_s::dict_s(const dict_s& other) : valid(other.valid) {
    obj = std::make_shared<map_t>();
    if (other.obj) {
        for (const auto& pair : *other.obj) {
            key_s new_key = pair.first;
            val_s new_val = pair.second;  // Uses copy constructor
            (*obj)[new_key] = new_val;
        }
    }
}

// Move constructor - OPTIMIZATION #1: Move Semantics
inline dict_s::dict_s(dict_s&& other) noexcept 
    : obj(std::move(other.obj)), valid(other.valid) {
    other.valid = false;
}

inline dict_s::~dict_s() {}

// Copy assignment
inline dict_s& dict_s::operator=(const dict_s& other) {
    if (this != &other) {
        valid = other.valid;
        obj = std::make_shared<map_t>();
        if (other.obj) {
            for (const auto& pair : *other.obj) {
                (*obj)[pair.first] = pair.second;
            }
        }
    }
    return *this;
}

// Move assignment - OPTIMIZATION #1: Move Semantics  
inline dict_s& dict_s::operator=(dict_s&& other) noexcept {
    if (this != &other) {
        obj = std::move(other.obj);
        valid = other.valid;
        other.valid = false;
    }
    return *this;
}

inline val_s& dict_s::operator[](const std::string& s) {
    key_s key;
    key.t = json_k_string_t;
    key.intg = 0;
    key.str = s;
    return (*obj)[key];
}

inline val_s& dict_s::operator[](int32_t i) {
    key_s key;
    key.t = json_k_integer_t;
    key.intg = i;
    key.str = "";
    return (*obj)[key];
}

// OPTIMIZATION #3: Optimized Serialization with String Buffer
inline std::string dict_s::dump(int indent) const {
    std::ostringstream oss;
    oss.str().reserve(1024);  // Pre-allocate reasonable buffer
    dump_internal(oss, indent, 0);
    return oss.str();
}

inline void dict_s::dump_internal(std::ostringstream& oss, int indent, int current_level) const {
    oss << '{';
    bool first = true;
    
    for (auto it = obj->begin(); it != obj->end(); ++it) {
        if (!first) oss << ',';
        first = false;
        
        if (indent >= 0) {
            oss << '\n' << std::string((current_level + 1) * indent, ' ');
        }
        
        // Write key
        if (it->first.t == json_k_string_t) {
            oss << '"' << it->first.str << '"';
        } else {
            oss << it->first.intg;
        }
        oss << ':';
        if (indent >= 0) oss << ' ';
        
        // Write value
        const val_s& val = it->second;
        switch (val.t) {
            case json_null_t:
                oss << "null";
                break;
            case json_bool_t:
                oss << (val.boolean ? "true" : "false");
                break;
            case json_integer_t:
                oss << val.intg;
                break;
            case json_double_t:
                oss << val.dbl;
                break;
            case json_string_t:
                oss << '"' << val.str << '"';
                break;
            case json_array:
                oss << '[';
                for (size_t i = 0; i < val.arr.size(); ++i) {
                    if (i > 0) oss << ',';
                    const val_s& item = *val.arr[i];
                    if (item.t == json_string_t) {
                        oss << '"' << item.str << '"';
                    } else if (item.t == json_integer_t) {
                        oss << item.intg;
                    } else if (item.t == json_double_t) {
                        oss << item.dbl;
                    } else if (item.t == json_bool_t) {
                        oss << (item.boolean ? "true" : "false");
                    } else if (item.t == json_null_t) {
                        oss << "null";
                    } else if (item.t == json_object_t && item.obj) {
                        item.obj->dump_internal(oss, indent, current_level + 1);
                    }
                }
                oss << ']';
                break;
            case json_object_t:
                if (val.obj) {
                    val.obj->dump_internal(oss, indent, current_level + 1);
                } else {
                    oss << "{}";
                }
                break;
        }
    }
    
    if (indent >= 0 && !first) {
        oss << '\n' << std::string(current_level * indent, ' ');
    }
    oss << '}';
}

// OPTIMIZATION #4: JSON Parsing
inline bool dict_s::parse(const std::string& json_str) {
    JSONParser parser(json_str);
    return parser.parse(*this);
}

// ==================== Comparison Operators ====================

#ifndef JSON_ORDERED
inline bool operator==(const key_s& first, const key_s& second) {
    return (first.t == second.t &&
            first.intg == second.intg &&
            first.str == second.str);
}

inline std::size_t json_key_hash::operator()(const key_s& k) const {
    using std::size_t;
    using std::hash;
    using std::string;
    
    return ((hash<int>()(k.t)
         ^ (hash<string>()(k.str) << 1)) >> 1)
         ^ (hash<int>()(k.intg) << 1);
}
#else
inline bool operator<(const key_s& first, const key_s& second) {
    if (first.t < second.t) return true;
    if (first.t > second.t) return false;
    if (first.t == json_k_integer_t) {
        return first.intg < second.intg;
    }
    if (first.t == json_k_string_t) {
        return first.str < second.str;
    }
    return false;
}
#endif

// ==================== Stream Operators (Backward Compatibility) ====================

inline std::ostream& operator<<(std::ostream& os, const dict_t& dict) {
    os << dict.dump(-1);  // No indentation for backward compatibility
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const val_t& val) {
    switch (val.t) {
        case json_null_t:
            os << "null";
            break;
        case json_bool_t:
            os << (val.boolean ? "true" : "false");
            break;
        case json_integer_t:
            os << val.intg;
            break;
        case json_double_t:
            os << val.dbl;
            break;
        case json_string_t:
            os << '"' << val.str << '"';
            break;
        case json_array:
            os << '[';
            for (size_t i = 0; i < val.arr.size(); ++i) {
                if (i > 0) os << ',';
                os << *val.arr[i];
            }
            os << ']';
            break;
        case json_object_t:
            if (val.obj) {
                os << *val.obj;
            } else {
                os << "{}";
            }
            break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const dict_key_t& k) {
    if (k.t == json_k_string_t) {
        os << '"' << k.str << '"';
    } else {
        os << k.intg;
    }
    return os;
}

// ==================== JSON Parser Implementation ====================

inline JSONParser::JSONParser(const std::string& json) 
    : cursor(json.c_str()), end(json.c_str() + json.size()) {}

inline void JSONParser::skip_whitespace() {
    while (cursor < end && std::isspace(*cursor)) {
        cursor++;
    }
}

inline bool JSONParser::parse(dict_s& dict) {
    skip_whitespace();
    return parse_object(dict);
}

inline bool JSONParser::parse(val_s& val) {
    skip_whitespace();
    return parse_value(val);
}

inline bool JSONParser::parse_value(val_s& val) {
    skip_whitespace();
    if (cursor >= end) return false;
    
    if (*cursor == '{') {
        val.t = json_object_t;
        val.obj = std::make_shared<dict_s>();
        return parse_object(*val.obj);
    } else if (*cursor == '[') {
        return parse_array(val);
    } else if (*cursor == '"') {
        val.t = json_string_t;
        return parse_string(val.str);
    } else if (*cursor == 't' || *cursor == 'f' || *cursor == 'n') {
        return parse_literal(val);
    } else if (*cursor == '-' || std::isdigit(*cursor)) {
        return parse_number(val);
    }
    
    return false;
}

inline bool JSONParser::parse_object(dict_s& dict) {
    skip_whitespace();
    if (cursor >= end || *cursor != '{') return false;
    cursor++; // Skip '{'
    
    skip_whitespace();
    if (cursor < end && *cursor == '}') {
        cursor++;
        return true; // Empty object
    }
    
    while (cursor < end) {
        skip_whitespace();
        
        // Parse key
        std::string key_str;
        int key_int = 0;
        bool is_string_key = true;
        
        if (*cursor == '"') {
            if (!parse_string(key_str)) return false;
        } else if (std::isdigit(*cursor) || *cursor == '-') {
            // Integer key
            char* endptr;
            key_int = std::strtol(cursor, &endptr, 10);
            cursor = endptr;
            is_string_key = false;
        } else {
            return false;
        }
        
        skip_whitespace();
        if (cursor >= end || *cursor != ':') return false;
        cursor++; // Skip ':'
        
        // Parse value
        val_s val;
        if (!parse_value(val)) return false;
        
        // Store in dict
        if (is_string_key) {
            dict[key_str] = std::move(val);
        } else {
            dict[key_int] = std::move(val);
        }
        
        skip_whitespace();
        if (cursor >= end) return false;
        
        if (*cursor == '}') {
            cursor++;
            return true;
        } else if (*cursor == ',') {
            cursor++;
        } else {
            return false;
        }
    }
    
    return false;
}

inline bool JSONParser::parse_array(val_s& val) {
    skip_whitespace();
    if (cursor >= end || *cursor != '[') return false;
    cursor++; // Skip '['
    
    val.t = json_array;
    val.arr.clear();
    
    skip_whitespace();
    if (cursor < end && *cursor == ']') {
        cursor++;
        return true; // Empty array
    }
    
    while (cursor < end) {
        val_s item;
        if (!parse_value(item)) return false;
        val.arr.push_back(std::make_shared<val_s>(std::move(item)));
        
        skip_whitespace();
        if (cursor >= end) return false;
        
        if (*cursor == ']') {
            cursor++;
            return true;
        } else if (*cursor == ',') {
            cursor++;
        } else {
            return false;
        }
    }
    
    return false;
}

inline bool JSONParser::parse_string(std::string& str) {
    if (cursor >= end || *cursor != '"') return false;
    cursor++; // Skip opening '"'
    
    str.clear();
    while (cursor < end) {
        if (*cursor == '"') {
            cursor++;
            return true;
        } else if (*cursor == '\\') {
            cursor++;
            if (cursor >= end) return false;
            switch (*cursor) {
                case '"': str += '"'; break;
                case '\\': str += '\\'; break;
                case '/': str += '/'; break;
                case 'b': str += '\b'; break;
                case 'f': str += '\f'; break;
                case 'n': str += '\n'; break;
                case 'r': str += '\r'; break;
                case 't': str += '\t'; break;
                default: return false;
            }
            cursor++;
        } else {
            str += *cursor;
            cursor++;
        }
    }
    
    return false;
}

inline bool JSONParser::parse_number(val_s& val) {
    char* endptr;
    
    // Try integer first
    long long int_val = std::strtoll(cursor, &endptr, 10);
    
    // Check if it's a float
    if (endptr < end && (*endptr == '.' || *endptr == 'e' || *endptr == 'E')) {
        double dbl_val = std::strtod(cursor, &endptr);
        if (endptr == cursor) return false;
        val.t = json_double_t;
        val.dbl = dbl_val;
        cursor = endptr;
        return true;
    }
    
    if (endptr == cursor) return false;
    val.t = json_integer_t;
    val.intg = int_val;
    cursor = endptr;
    return true;
}

inline bool JSONParser::parse_literal(val_s& val) {
    if (cursor + 4 <= end && std::strncmp(cursor, "null", 4) == 0) {
        val.t = json_null_t;
        cursor += 4;
        return true;
    } else if (cursor + 4 <= end && std::strncmp(cursor, "true", 4) == 0) {
        val.t = json_bool_t;
        val.boolean = true;
        cursor += 4;
        return true;
    } else if (cursor + 5 <= end && std::strncmp(cursor, "false", 5) == 0) {
        val.t = json_bool_t;
        val.boolean = false;
        cursor += 5;
        return true;
    }
    return false;
}

#endif // DICT_CPP_H
