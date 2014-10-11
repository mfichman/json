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
#include "json/Error.hpp"
#include "json/json.hpp"

namespace json {

void parseWhitespace(std::istream& ss) {
    while (isspace(ss.peek())) {
        ss.get();
    }
}

void match(std::istream& ss, std::string const& value) {
    for (auto ch : value) {
        if (ss.get() != ch) {
            throw ParseError(std::string("expected '")+ch+"'");
        }
    }
}

std::string parseUnicodeEscapeSequence(std::istream& ss) {
    assert(!"not implemented");
    return "";
}

std::string parseEscapeSequence(std::istream& ss) {
    auto ch = ss.get();
    switch (ch) {
    case '"': return "\"";
    case '\\': return "\\";
    case '/': return "/";
    case 'b': return "\b";
    case 'f': return "\f";
    case 'n': return "\n";
    case 'r': return "\r";
    case 't': return "\t";
    case 'u': return parseUnicodeEscapeSequence(ss);
    default: throw ParseError(std::string("unexpected escape sequence: '\\")+char(ch)+"'");
    }
}

std::string parseString(std::istream& ss) {
    // UTF-8
    if (ss.get() != '"') {
        throw ParseError("expected '\"'");
    }

    std::string token;
    for (auto ch = ss.get(); ch; ch = ss.get()) {
        switch (ch) {
        case '\\': token += parseEscapeSequence(ss); break;
        case '"': return token;
        default: token += char(ch); break;
        }
    }
    throw ParseError("unterminated string");
}

Object parseObject(std::istream& ss) {
    // FixMe: Do not copy map on return
    if (ss.get() != '{') {
        throw ParseError("expected '{'");
    }
    parseWhitespace(ss);
    Object object;
    for (auto ch = ss.peek(); ch; ch = ss.peek()) {
        if (ch == '}') {
            ss.get();
            return object;
        }
        std::string name = parseString(ss);
        parseWhitespace(ss);
        if (ss.get() != ':') {
            throw ParseError("expected ':'");
        }
        parseWhitespace(ss);
        Value value = load(ss);
        object[name] = value;
        parseWhitespace(ss);
        ch = ss.get();
		if (ch == ',') {
			// Pass
			parseWhitespace(ss);
		} else if (ch == '}') {
			return object;
		} else {
            throw ParseError("expected ',' or '}'");
        }
    }
    throw ParseError("unterminated object");
}

Value parseArray(std::istream& ss) {
    if (ss.get() != '[') {
        throw ParseError("expected '['");
    }
    parseWhitespace(ss);
    Array array;
    for (auto ch = ss.peek(); ch; ch = ss.peek()) {
        if (ch == ']') {
            ss.get();
            return array;
        }
        Value value = load(ss);
        array.push_back(value);
        parseWhitespace(ss);
        ch = ss.get();
		if (ch == ',') {
			// Pass
			parseWhitespace(ss);
		} else if (ch == ']') {
			return array;
		} else {
            throw ParseError("expected ',' or ']'");
        }
    }
    return Value();
}

Value parseNumber(std::istream& ss) {
    double value = 0;
    ss >> value;
    return Value(value);
}

bool parseTrue(std::istream& ss) {
    match(ss, "true");
    return true;
}

bool parseFalse(std::istream& ss) {
    match(ss, "false");
    return false;
}

std::nullptr_t parseNull(std::istream& ss) {
    match(ss, "null");
    return nullptr;
}

Value load(std::istream& ss) {
// Parse a JSON entity, using the inputstream ss
    parseWhitespace(ss);
    switch (ss.peek()) {
    case '{': return parseObject(ss);
    case '[': return parseArray(ss);
    case '"': return parseString(ss);
    case 't': return parseTrue(ss);
    case 'f': return parseFalse(ss);
    case 'n': return parseNull(ss);
    case '-': return parseNumber(ss);
    default: 
        if(isdigit(ss.peek())) {
            return parseNumber(ss);
        } else {
            throw ParseError(std::string("unexpected token: '")+char(ss.peek())+"'");
        }
    }
}

Value loads(std::string const& str) {
    std::stringstream ss(str);
    return load(ss);
}

void dump(Value const& value, std::ostream& ss) {
// Dumps the json value into the specified output stream.
    switch (value.type()) {
    case STRING: ss << '"'+value.string()+'"'; break;
    case NUMBER: ss << value.number(); break;
    case ARRAY: {
        ss << '[';
        auto first = true;
        for(auto item : value.array()) {
            if(!first) {
                ss << ',';   
            }
            first = false;
            dump(item,ss);
        }
        ss << ']';
        break;
    }
    case OBJECT: {
        ss << '{'; 
        auto first = true;
        for(auto pair : value.object()) {
            if(!first) {
                ss << ',';
            }
            first = false;
            ss << '"' << pair.first << "\":";
            dump(pair.second,ss);
        }
        ss << '}';
        break;
    }
    case NIL: ss << "null"; break;
    case BOOLEAN: ss << (value.boolean() ? "true" : "false"); break;
    default: assert(!"invalid type");
    }
}

std::string dumps(Value const& value) {
// Dumps a json value into a string
    std::stringstream ss;
    dump(value,ss);
    return ss.str();
}

}
