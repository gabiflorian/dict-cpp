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

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>
#include "../include/dict/dict.h"

// Simple test framework
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    std::cout << "Running " #name "... "; \
    test_##name(); \
    std::cout << "PASSED" << std::endl; \
    passed_tests++; \
} while(0)

int passed_tests = 0;
int total_tests = 0;

// Helper function to convert dict to string
std::string dict_to_string(dict_t& d) {
    std::ostringstream oss;
    oss << d;
    return oss.str();
}

// Helper function to convert val to string
std::string val_to_string(val_t& v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

// Helper function to check if string contains all substrings
bool contains_all(const std::string& str, const std::vector<std::string>& patterns) {
    for (const auto& pattern : patterns) {
        if (str.find(pattern) == std::string::npos) {
            return false;
        }
    }
    return true;
}

// Helper to count occurrences of character in string
int count_char(const std::string& str, char c) {
    return std::count(str.begin(), str.end(), c);
}

// ==================== TESTS ====================

TEST(empty_dict_creation) {
    dict_t d;
    assert(d.valid);
    assert(d.obj != nullptr);
    std::string result = dict_to_string(d);
    assert(result == "{}");
}

TEST(string_key_integer_value) {
    dict_t d;
    d["age"] = 25;
    std::string result = dict_to_string(d);
    assert(result == "{\"age\":25}");
}

TEST(string_key_string_value) {
    dict_t d;
    d["name"] = "John";
    std::string result = dict_to_string(d);
    assert(result == "{\"name\":\"John\"}");
}

TEST(integer_key_integer_value) {
    dict_t d;
    d[42] = 100;
    std::string result = dict_to_string(d);
    assert(result == "{42:100}");
}

TEST(integer_key_string_value) {
    dict_t d;
    d[1] = "first";
    std::string result = dict_to_string(d);
    assert(result == "{1:\"first\"}");
}

TEST(mixed_key_types) {
    dict_t d;
    d["string_key"] = 1;
    d[2] = "integer_key";
    std::string result = dict_to_string(d);
    // Check both keys exist (order may vary with unordered_map)
    assert(contains_all(result, {"2:\"integer_key\"", "\"string_key\":1"}));
    assert(result[0] == '{' && result[result.size()-1] == '}');
}

TEST(multiple_string_keys) {
    dict_t d;
    d["first"] = 1;
    d["second"] = 2;
    d["third"] = 3;
    std::string result = dict_to_string(d);
    // Check all keys exist (order may vary)
    assert(contains_all(result, {"\"first\":1", "\"second\":2", "\"third\":3"}));
    assert(count_char(result, ',') == 2);  // Should have 2 commas for 3 items
}

TEST(generic_array_integers) {
    dict_t d;
    d["numbers"] = std::vector<int>{1, 2, 3, 4, 5};
    assert(d["numbers"].is_array());
    assert(d["numbers"].size() == 5);
    assert(d["numbers"].at(0).intg == 1);
}

TEST(generic_array_strings) {
    dict_t d;
    d["tags"] = std::vector<std::string>{"admin", "user", "verified"};
    assert(d["tags"].is_array());
    assert(d["tags"].size() == 3);
    assert(d["tags"].at(0).str == "admin");
}

TEST(generic_array_doubles) {
    dict_t d;
    d["scores"] = std::vector<double>{1.1, 2.2, 3.3};
    assert(d["scores"].is_array());
    assert(d["scores"].size() == 3);
}

TEST(mixed_type_array) {
    val_t mixed;
    mixed.push_back(42);
    mixed.push_back("text");
    mixed.push_back(3.14);
    mixed.push_back(true);
    
    assert(mixed.is_array());
    assert(mixed.size() == 4);
    assert(mixed.at(0).is_int());
    assert(mixed.at(1).is_string());
    assert(mixed.at(2).is_double());
    assert(mixed.at(3).is_bool());
}

TEST(array_empty) {
    dict_t d;
    d["empty"] = std::vector<int>{};
    std::string result = dict_to_string(d);
    assert(result == "{\"empty\":[]}");
}

TEST(array_modification) {
    dict_t d;
    d["list"] = std::vector<int>{10, 20};
    d["list"].push_back(30);
    d["list"].push_back(40);
    assert(d["list"].size() == 4);
}

TEST(nested_dict_creation) {
    dict_t inner;
    inner["inner_key"] = 42;
    
    dict_t outer;
    outer["nested"] = inner;
    
    std::string result = dict_to_string(outer);
    assert(result == "{\"nested\":{\"inner_key\":42}}");
}

TEST(nested_dict_access) {
    dict_t a;
    a["x"] = 10;
    
    dict_t b;
    b["data"] = a;
    b["data"]["y"] = 20;
    
    std::string result = dict_to_string(b);
    // Check nested structure (order may vary)
    assert(contains_all(result, {"\"data\":{", "\"x\":10", "\"y\":20"}));
}

TEST(deep_nesting) {
    dict_t level3;
    level3["value"] = 3;
    
    dict_t level2;
    level2["level3"] = level3;
    
    dict_t level1;
    level1["level2"] = level2;
    
    std::string result = dict_to_string(level1);
    assert(result == "{\"level2\":{\"level3\":{\"value\":3}}}");
}

TEST(copy_independence) {
    dict_t a;
    a["key"] = 10;
    
    dict_t b;
    b["copy"] = a;
    
    // Modify original after copying
    a["key"] = 20;
    a["new"] = 30;
    
    // b should still have the old value
    std::string result_b = dict_to_string(b);
    assert(result_b == "{\"copy\":{\"key\":10}}");
}

TEST(move_semantics) {
    dict_t a;
    a["key"] = 100;
    
    dict_t b = std::move(a);
    assert(b["key"].intg == 100);
}

TEST(type_checking) {
    dict_t d;
    d["null_val"] = val_t();
    d["bool_val"] = true;
    d["int_val"] = 42;
    d["double_val"] = 3.14;
    d["string_val"] = "hello";
    d["array_val"] = std::vector<int>{1, 2, 3};
    
    dict_t nested;
    nested["x"] = 10;
    d["object_val"] = nested;
    
    assert(d["null_val"].is_null());
    assert(d["bool_val"].is_bool());
    assert(d["int_val"].is_int());
    assert(d["double_val"].is_double());
    assert(d["string_val"].is_string());
    assert(d["array_val"].is_array());
    assert(d["object_val"].is_object());
}

TEST(json_parsing_simple) {
    std::string json = R"({"name":"Alice","age":25})";
    dict_t d(json);
    
    assert(d.valid);
    assert(d["name"].str == "Alice");
    assert(d["age"].intg == 25);
}

TEST(json_parsing_nested) {
    std::string json = R"({
        "user": {
            "name": "Bob",
            "age": 30
        }
    })";
    dict_t d(json);
    
    assert(d.valid);
    assert(d["user"].is_object());
}

TEST(json_parsing_array) {
    std::string json = R"({"scores":[95,87,92]})";
    dict_t d(json);
    
    assert(d.valid);
    assert(d["scores"].is_array());
    assert(d["scores"].size() == 3);
}

TEST(json_serialization_compact) {
    dict_t d;
    d["name"] = "Alice";
    d["age"] = 25;
    
    std::string json = d.dump();
    assert(json.find("name") != std::string::npos);
    assert(json.find("Alice") != std::string::npos);
}

TEST(json_serialization_pretty) {
    dict_t d;
    d["name"] = "Alice";
    
    std::string json = d.dump(2);
    assert(json.find("\n") != std::string::npos);  // Has newlines
}

TEST(bool_values) {
    dict_t d;
    d["active"] = true;
    d["verified"] = false;
    
    assert(d["active"].boolean == true);
    assert(d["verified"].boolean == false);
}

TEST(double_values) {
    dict_t d;
    d["pi"] = 3.14159;
    d["e"] = 2.71828;
    
    assert(d["pi"].dbl > 3.14 && d["pi"].dbl < 3.15);
}

TEST(numeric_string_keys) {
    dict_t d;
    d["1"] = "string_key_1";
    d[1] = "integer_key_1";
    
    std::string result = dict_to_string(d);
    // These are different keys! (order may vary)
    assert(contains_all(result, {"1:\"integer_key_1\"", "\"1\":\"string_key_1\""}));
}

TEST(empty_string_key) {
    dict_t d;
    d[""] = 42;
    std::string result = dict_to_string(d);
    assert(result == "{\"\":42}");
}

TEST(large_integer_keys) {
    dict_t d;
    d[0] = "zero";
    d[1000] = "thousand";
    d[-500] = "negative";
    
    std::string result = dict_to_string(d);
    // Check all keys exist (order may vary)
    assert(contains_all(result, {"-500:\"negative\"", "0:\"zero\"", "1000:\"thousand\""}));
    assert(count_char(result, ',') == 2);
}

// ==================== MAIN ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "       dict-cpp Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;
    
    try {
        RUN_TEST(empty_dict_creation);
        RUN_TEST(string_key_integer_value);
        RUN_TEST(string_key_string_value);
        RUN_TEST(integer_key_integer_value);
        RUN_TEST(integer_key_string_value);
        RUN_TEST(mixed_key_types);
        RUN_TEST(multiple_string_keys);
        RUN_TEST(generic_array_integers);
        RUN_TEST(generic_array_strings);
        RUN_TEST(generic_array_doubles);
        RUN_TEST(mixed_type_array);
        RUN_TEST(array_empty);
        RUN_TEST(array_modification);
        RUN_TEST(nested_dict_creation);
        RUN_TEST(nested_dict_access);
        RUN_TEST(deep_nesting);
        RUN_TEST(copy_independence);
        RUN_TEST(move_semantics);
        RUN_TEST(type_checking);
        RUN_TEST(json_parsing_simple);
        RUN_TEST(json_parsing_nested);
        RUN_TEST(json_parsing_array);
        RUN_TEST(json_serialization_compact);
        RUN_TEST(json_serialization_pretty);
        RUN_TEST(bool_values);
        RUN_TEST(double_values);
        RUN_TEST(numeric_string_keys);
        RUN_TEST(empty_string_key);
        RUN_TEST(large_integer_keys);
        
        total_tests = passed_tests;
        
    } catch (const std::exception& e) {
        std::cerr << std::endl << "FAILED with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << std::endl << "FAILED with unknown exception" << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Results: " << passed_tests << "/" << total_tests << " tests passed" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return (passed_tests == total_tests) ? 0 : 1;
}

