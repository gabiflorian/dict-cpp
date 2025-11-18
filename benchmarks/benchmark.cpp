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
#include <iomanip>
#include <chrono>
#include <vector>
#include <string>
#include "../include/dict/dict.h"

// If nlohmann/json is available, uncomment to compare
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#define HAVE_NLOHMANN

using namespace std::chrono;

class Timer {
private:
    high_resolution_clock::time_point start_time;
    
public:
    Timer() {
        start_time = high_resolution_clock::now();
    }
    
    double elapsed_ms() {
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        return duration.count() / 1000.0;
    }
};

void print_result(const std::string& test_name, double dict_time, double other_time = 0) {
    std::cout << std::left << std::setw(40) << test_name;
    
    if (other_time > 0) {
        std::cout << std::right << std::setw(12) << std::fixed << std::setprecision(2) << other_time << " ms";
        std::cout << std::setw(14) << std::fixed << std::setprecision(2) << dict_time << " ms";
        double speedup = other_time / dict_time;
        std::cout << std::setw(12) << std::fixed << std::setprecision(2) << speedup << "x";
    } else {
        std::cout << std::right << std::setw(12) << std::fixed << std::setprecision(2) << dict_time << " ms";
    }
    std::cout << std::endl;
}

void benchmark_simple_insertions() {
    const int N = 10000;
    
    double dict_time, json_time = 0;
    
    {
        Timer timer;
        dict_t d;
        for (int i = 0; i < N; i++) {
            std::string key = "key_" + std::to_string(i);
            d[key] = i;
        }
        dict_time = timer.elapsed_ms();
    }
    
#ifdef HAVE_NLOHMANN
    {
        Timer timer;
        json j;
        for (int i = 0; i < N; i++) {
            std::string key = "key_" + std::to_string(i);
            j[key] = i;
        }
        json_time = timer.elapsed_ms();
    }
#endif
    
    print_result("Simple insertions (10k items)", dict_time, json_time);
}

void benchmark_integer_keys() {
    const int N = 10000;
    
    double dict_time, json_time = 0;
    
    {
        Timer timer;
        dict_t d;
        for (int i = 0; i < N; i++) {
            d[i] = i * 2;
        }
        dict_time = timer.elapsed_ms();
    }
    
#ifdef HAVE_NLOHMANN
    {
        Timer timer;
        json j;
        for (int i = 0; i < N; i++) {
            j[std::to_string(i)] = i * 2;
        }
        json_time = timer.elapsed_ms();
    }
#endif
    
    print_result("Integer key insertions (10k items)", dict_time, json_time);
}

void benchmark_mixed_types() {
    const int N = 3000;
    
    {
        Timer timer;
        dict_t d;
        for (int i = 0; i < N; i++) {
            std::string key = "key_" + std::to_string(i);
            if (i % 3 == 0) {
                d[key] = i;
            } else if (i % 3 == 1) {
                d[key] = "value_" + std::to_string(i);
            } else {
                d[key] = std::vector<int>{i, i+1, i+2};
            }
        }
        print_result("Mixed types (3k items)", timer.elapsed_ms());
    }
}

void benchmark_nested_objects() {
    const int N = 1000;
    
    double dict_time, json_time = 0;
    
    {
        Timer timer;
        dict_t root;
        for (int i = 0; i < N; i++) {
            dict_t inner;
            inner["id"] = i;
            inner["name"] = "item_" + std::to_string(i);
            std::string key = "obj_" + std::to_string(i);
            root[key] = inner;
        }
        dict_time = timer.elapsed_ms();
    }
    
#ifdef HAVE_NLOHMANN
    {
        Timer timer;
        json root;
        for (int i = 0; i < N; i++) {
            json inner;
            inner["id"] = i;
            inner["name"] = "item_" + std::to_string(i);
            std::string key = "obj_" + std::to_string(i);
            root[key] = inner;
        }
        json_time = timer.elapsed_ms();
    }
#endif
    
    print_result("Nested objects (1k items)", dict_time, json_time);
}

void benchmark_serialization() {
    const int N = 1000;
    
    double dict_time, json_time = 0;
    
    dict_t d;
    for (int i = 0; i < N; i++) {
        std::string key = "key_" + std::to_string(i);
        d[key] = i;
    }
    
    {
        Timer timer;
        std::string result = d.dump();
        dict_time = timer.elapsed_ms();
    }
    
#ifdef HAVE_NLOHMANN
    json j;
    for (int i = 0; i < N; i++) {
        std::string key = "key_" + std::to_string(i);
        j[key] = i;
    }
    
    {
        Timer timer;
        std::string result = j.dump();
        json_time = timer.elapsed_ms();
    }
#endif
    
    print_result("Serialization (1k items)", dict_time, json_time);
}

void benchmark_parsing() {
    std::string json_str = R"({
        "name": "Alice",
        "age": 25,
        "scores": [95, 87, 92],
        "address": {
            "city": "NYC",
            "zip": 10001
        }
    })";
    
    double dict_time, json_time = 0;
    
    {
        Timer timer;
        for (int i = 0; i < 1000; i++) {
            dict_t d(json_str);
        }
        dict_time = timer.elapsed_ms();
    }
    
#ifdef HAVE_NLOHMANN
    {
        Timer timer;
        for (int i = 0; i < 1000; i++) {
            json j = json::parse(json_str);
        }
        json_time = timer.elapsed_ms();
    }
#endif
    
    print_result("JSON parsing (1000x)", dict_time, json_time);
}

void benchmark_array_operations() {
    const int N = 1000;
    const int ARRAY_SIZE = 100;
    
    {
        Timer timer;
        dict_t d;
        for (int i = 0; i < N; i++) {
            std::string key = "array_" + std::to_string(i);
            std::vector<int> arr;
            for (int j = 0; j < ARRAY_SIZE; j++) {
                arr.push_back(j);
            }
            d[key] = arr;
        }
        print_result("Array operations (1k x 100 items)", timer.elapsed_ms());
    }
}

void benchmark_copy_operations() {
    const int N = 500;
    
    double dict_time, json_time = 0;
    
    dict_t d_src;
    for (int i = 0; i < N; i++) {
        std::string key = "key_" + std::to_string(i);
        d_src[key] = i;
    }
    
    {
        Timer timer;
        for (int i = 0; i < 100; i++) {
            dict_t d_copy;
            d_copy["data"] = d_src;
        }
        dict_time = timer.elapsed_ms();
    }
    
#ifdef HAVE_NLOHMANN
    json j_src;
    for (int i = 0; i < N; i++) {
        std::string key = "key_" + std::to_string(i);
        j_src[key] = i;
    }
    
    {
        Timer timer;
        for (int i = 0; i < 100; i++) {
            json j_copy;
            j_copy["data"] = j_src;
        }
        json_time = timer.elapsed_ms();
    }
#endif
    
    print_result("Copy operations (100x500 items)", dict_time, json_time);
}

void benchmark_move_operations() {
    const int N = 1000;
    
    {
        Timer timer;
        for (int i = 0; i < 1000; i++) {
            dict_t d;
            for (int j = 0; j < N; j++) {
                d["key_" + std::to_string(j)] = j;
            }
            dict_t moved = std::move(d);
        }
        print_result("Move operations (1000x1000 items)", timer.elapsed_ms());
    }
}

void benchmark_random_access() {
    const int N = 5000;
    
    double dict_time, json_time = 0;
    
    dict_t d;
    for (int i = 0; i < N; i++) {
        std::string key = "key_" + std::to_string(i);
        d[key] = i;
    }
    
    {
        Timer timer;
        volatile int sum = 0;
        for (int i = 0; i < N; i++) {
            std::string key = "key_" + std::to_string(i);
            sum += d[key].intg;
        }
        dict_time = timer.elapsed_ms();
    }
    
#ifdef HAVE_NLOHMANN
    json j;
    for (int i = 0; i < N; i++) {
        std::string key = "key_" + std::to_string(i);
        j[key] = i;
    }
    
    {
        Timer timer;
        volatile int sum = 0;
        for (int i = 0; i < N; i++) {
            std::string key = "key_" + std::to_string(i);
            sum += j[key].get<int>();
        }
        json_time = timer.elapsed_ms();
    }
#endif
    
    print_result("Random access (5k lookups)", dict_time, json_time);
}

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              dict-cpp Performance Benchmarks               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    
    std::cout << std::string(70, '=') << std::endl;
    std::cout << std::left << std::setw(40) << "Benchmark";
#ifdef HAVE_NLOHMANN
    std::cout << std::right << std::setw(12) << "nlohmann";
    std::cout << std::setw(14) << "dict-cpp";
    std::cout << std::setw(12) << "Speedup";
#else
    std::cout << std::right << std::setw(12) << "dict-cpp";
#endif
    std::cout << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    std::cout << "\n[1/10] Simple insertions..." << std::endl;
    benchmark_simple_insertions();
    
    std::cout << "\n[2/10] Integer key insertions..." << std::endl;
    benchmark_integer_keys();
    
    std::cout << "\n[3/10] Mixed value types..." << std::endl;
    benchmark_mixed_types();
    
    std::cout << "\n[4/10] Nested objects..." << std::endl;
    benchmark_nested_objects();
    
    std::cout << "\n[5/10] Serialization..." << std::endl;
    benchmark_serialization();
    
    std::cout << "\n[6/10] JSON parsing..." << std::endl;
    benchmark_parsing();
    
    std::cout << "\n[7/10] Array operations..." << std::endl;
    benchmark_array_operations();
    
    std::cout << "\n[8/10] Copy operations..." << std::endl;
    benchmark_copy_operations();
    
    std::cout << "\n[9/10] Move operations..." << std::endl;
    benchmark_move_operations();
    
    std::cout << "\n[10/10] Random access..." << std::endl;
    benchmark_random_access();
    
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "\n✓ Benchmarks completed!\n" << std::endl;
    
    std::cout << "Notes:\n";
    std::cout << "  • Times are in milliseconds (ms)\n";
    std::cout << "  • Results may vary based on system load\n";
#ifdef HAVE_NLOHMANN
    std::cout << "  • Speedup shows how many times faster dict-cpp is\n";
    std::cout << "  • Speedup > 1.0 means dict-cpp is faster\n";
    std::cout << "  • Speedup < 1.0 means nlohmann is faster\n";
#else
    std::cout << "  • To compare with nlohmann, uncomment #define HAVE_NLOHMANN\n";
#endif
    std::cout << std::endl;
    
    return 0;
}

