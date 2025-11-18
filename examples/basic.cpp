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
    cout << "=== dict-cpp Basic Example ===" << endl << endl;
    
    // Create a dictionary
    dict_t person;
    person["name"] = "Alice";
    person["age"] = 25;
    person["active"] = true;
    person["score"] = 98.5;
    
    // Integer keys (unique feature!)
    person[0] = "admin";
    person[1] = "verified";
    
    cout << "Person dict:" << endl;
    cout << person.dump(2) << endl << endl;
    
    // Access values
    cout << "Name: " << person["name"].str << endl;
    cout << "Age: " << person["age"].intg << endl;
    cout << "Active: " << (person["active"].boolean ? "yes" : "no") << endl;
    cout << "Score: " << person["score"].dbl << endl;
    cout << "Role: " << person[0].str << endl << endl;
    
    // Type checking
    if (person["age"].is_int()) {
        cout << "Age is an integer!" << endl;
    }
    
    return 0;
}

