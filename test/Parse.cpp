/*
 * Copyright (c) 2014 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "json/Common.hpp"
#include "json/json.hpp"
#include <iostream>

void testString() {
    auto val = json::loads("\"foobar\"");
    assert(val.string() == "foobar");

    val = json::loads("\" \\\"\\\\\\/\\b\\f\\n\\r\\t \"");
    assert(val.string() == " \"\\/\b\f\n\r\t ");

    // test unicode...
}

void testBoolean() {
    auto val = json::loads("true");
    assert(val.isBoolean());
    assert(val.boolean() == true);

    val = json::loads("false");
    assert(val.isBoolean());
    assert(val.boolean() == false);
}

void testNull() {
    auto val = json::loads("null");
    assert(val.isNull() == true);
}

void testObject() {
    auto val = json::loads("{ \"foo\": true }");
    assert(val["foo"].boolean() == true);

    val = json::loads("{ \"foo\": \"bar\", \"baz\": null }");
    assert(val["foo"].string() == "bar");
    assert(val["baz"].isNull());

    val = json::loads("{ \"foo\": { \"bar\": true, \"bingo\": null }}");
    assert(val["foo"].isObject());
    assert(val["foo"]["bar"].boolean() == true);
    assert(val["foo"]["bingo"].isNull());

    val = json::loads(" { } ");
    assert(val.isObject());

    val = json::loads(" { \"foo\": [ true ]} ");
    assert(val.isObject());
    assert(val["foo"].isArray());
    assert(val["foo"][0].isBoolean());
}

void testArray() {
    auto val = json::loads("[ true ]");
    assert(val.isArray());
    assert(val[0].boolean() == true);

    val = json::loads("[ true, \"str\", null ]");
    assert(val.isArray());
    assert(val[0].boolean() == true);
    assert(val[1].string() == "str");
    assert(val[2].isNull());

    val = json::loads("[ [ true, \"str\" ], null ]");
    assert(val.isArray());
    assert(val[0].isArray());
    assert(val[0][0].boolean() == true);
    assert(val[0][1].string() == "str");
    assert(val[1].isNull());

    val = json::loads("[{ \"foo\": true }]");
    assert(val.isArray());
    assert(val[0].isObject());
    assert(val[0]["foo"].boolean() == true);
}

void testNumber() {
    auto val = json::loads("100"); 
    assert(val.number() == 100);

    val = json::loads("100.192"); 
    assert(val.number() == 100.192);

    val = json::loads("-100"); 
    assert(val.number() == -100);

    val = json::loads("100e10"); 
    assert(val.number() == 100e10);

    val = json::loads("100E10"); 
    assert(val.number() == 100E10);

    val = json::loads("100E+10"); 
    assert(val.number() == 100E+10);

    val = json::loads("100E-10"); 
    assert(val.number() == 100E-10);

    val = json::loads("100.1E-10"); 
    assert(val.number() == 100.1E-10);
}

int main() {
    testString();
    testBoolean();
    testNull();
    testObject();
    testArray();
    testNumber();
    return 0;
}
