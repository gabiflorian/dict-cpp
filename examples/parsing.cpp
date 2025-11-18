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
#include "../include/dict/dict.h"

using namespace std;

int main() {
    cout << "=== dict-cpp JSON Parsing Example ===" << endl << endl;
    
    // Simple JSON parsing
    string simple_json = R"({"name":"Bob","age":30,"active":true})";
    dict_t simple(simple_json);
    
    cout << "Parsed simple JSON:" << endl;
    cout << simple.dump(2) << endl << endl;
    
    // Complex nested JSON
    string complex_json = R"({
        "user": {
            "id": 123,
            "name": "Alice",
            "email": "alice@example.com",
            "scores": [95, 87, 92],
            "verified": true
        },
        "metadata": {
            "timestamp": 1234567890,
            "version": "2.0"
        }
    })";
    
    dict_t complex(complex_json);
    
    cout << "Parsed complex JSON:" << endl;
    cout << complex.dump(2) << endl << endl;
    
    // Access nested data
    cout << "User name: " << complex["user"]["name"].str << endl;
    cout << "User ID: " << complex["user"]["id"].intg << endl;
    cout << "Verified: " << (complex["user"]["verified"].boolean ? "yes" : "no") << endl;
    
    // Access array
    if (complex["user"]["scores"].is_array()) {
        cout << "Scores: ";
        for (size_t i = 0; i < complex["user"]["scores"].size(); i++) {
            cout << complex["user"]["scores"].at(i).intg;
            if (i < complex["user"]["scores"].size() - 1) cout << ", ";
        }
        cout << endl;
    }
    
    return 0;
}

