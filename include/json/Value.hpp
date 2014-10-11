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

#pragma once

#include "json/Common.hpp"
#include "json/Error.hpp"

namespace json {

class Any;

enum Type { STRING, NUMBER, ARRAY, OBJECT, NIL, BOOLEAN };

class Value {
public:
    typedef std::unordered_map<std::string,Value> Object;
    typedef std::vector<Value> Array;
    typedef std::string String;
    typedef double Number;
    typedef std::nullptr_t Null;
    typedef bool Boolean;

    Value();
    Value(char const* value) : Value(std::string(value)) {}
    Value(String const& value);
    Value(Array const& value);
    Value(Object const& value);
    Value(Null value);
    Value(Boolean value);
    Value(double value);
    Value(int value);
    Value(std::initializer_list<Object::value_type> init) : Value(Object(init)) {}
     
    String const& string() const;
    Number number() const;
    Array const& array() const;
    Object const& object() const;
    Boolean boolean() const;

    Value operator[](std::string const& name) const;
    Value operator[](size_t index) const;

    bool isString() const;
    bool isNumber() const;
    bool isArray() const;
    bool isObject() const;
    bool isNull() const;
    bool isBoolean() const;

    Type type() const;

private:
    template <typename T>
    Value(T const& value);

    Ptr<Any> container_;
};

typedef std::unordered_map<std::string,Value> Object;
typedef std::vector<Value> Array;
typedef std::string String;
typedef double Number;
typedef std::nullptr_t Null;
typedef bool Boolean;


class Any {
public:
    Any(Type type) : type_(type) {}
    virtual ~Any() {};
    virtual String const& string() const=0;
    virtual Number number() const=0;
    virtual Array const& array() const=0;
    virtual Object const& object() const=0;
    virtual Boolean boolean() const=0;

    virtual bool isString() const=0;
    virtual bool isNumber() const=0;
    virtual bool isArray() const=0;
    virtual bool isObject() const=0;
    virtual bool isNull() const=0;
    virtual bool isBoolean() const=0;

    Type type() const { return type_; }

private:
    Type type_;
};

template <typename T> inline Type typeOf();
template <> inline Type typeOf<String>() { return STRING; }
template <> inline Type typeOf<Number>() { return NUMBER; }
template <> inline Type typeOf<Array>() { return ARRAY; }
template <> inline Type typeOf<Object>() { return OBJECT; }
template <> inline Type typeOf<Null>() { return NIL; }
template <> inline Type typeOf<Boolean>() { return BOOLEAN; }

template <typename T>
class Container : public Any {
public:
    typedef T Type;
    Container(T const& value) : Any(typeOf<T>()), value_(value) {}
    Container() : Any(typeOf<T>()), value_{} {};

    template <typename V> V const& as() const { throw TypeError(); }
    template <typename V> bool is() const { return false; }

    virtual String const& string() const { return as<String>(); }
    virtual Number number() const { return as<Number>(); }
    virtual Array const& array() const { return as<Array>(); }
    virtual Object const& object() const { return as<Object>(); }
    virtual Boolean boolean() const { return as<Boolean>(); }

    virtual bool isString() const { return is<String>(); }
    virtual bool isNumber() const { return is<Number>(); }
    virtual bool isArray() const { return is<Array>(); }
    virtual bool isObject() const { return is<Object>(); }
    virtual bool isNull() const { return is<Null>(); }
    virtual bool isBoolean() const { return is<Boolean>(); }

private:
    T value_;
};

template <typename T>
Value::Value(T const& value) : container_(new Container<T>(value)) {} 


// Sometimes, C++ is really, really lame...
template <> template <> inline String const& Container<String>::as<String>() const { return value_; }
template <> template <> inline bool Container<String>::is<String>() const { return true; }

template <> template <> inline Number const& Container<Number>::as<Number>() const { return value_; }
template <> template <> inline bool Container<Number>::is<Number>() const { return true; }

template <> template <> inline Array const& Container<Array>::as<Array>() const { return value_; }
template <> template <> inline bool Container<Array>::is<Array>() const { return true; }

template <> template <> inline Object const& Container<Object>::as<Object>() const { return value_; }
template <> template <> inline bool Container<Object>::is<Object>() const { return true; }

template <> template <> inline Boolean const& Container<Boolean>::as<Boolean>() const { return value_; }
template <> template <> inline bool Container<Boolean>::is<Boolean>() const { return true; }

template <> template <> inline Null const& Container<Null>::as<Null>() const { return value_; }
template <> template <> inline bool Container<Null>::is<Null>() const { return true; }




}
