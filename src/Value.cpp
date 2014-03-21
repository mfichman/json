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
#include "json/Value.hpp"

namespace json {

Value::Value() : container_(new Container<Null>) {
}

Value::Value(String const& value) : container_(new Container<String>(value)) {
}

Value::Value(double value) : container_(new Container<Number>(value)) {
}

Value::Value(int value) : container_(new Container<Number>(value)) {
}

Value::Value(Boolean value) : container_(new Container<Boolean>(value)) {
}

Value::Value(Null value) : container_(new Container<Null>(value)) {
}

Value::Value(Object const& value) : container_(new Container<Object>(value)) {
}

Value::Value(Array const& value) : container_(new Container<Array>(value)) {
}

Value::String const& Value::string() const {
    return container_->string();
}

Value::Number Value::number() const {
    return container_->number();
}

Value::Array const& Value::array() const {
    return container_->array();
}

Value::Object const& Value::object() const {
    return container_->object();
}

Value::Boolean Value::boolean() const {
    return container_->boolean();
}

bool Value::isString() const { return container_->isString(); }
bool Value::isNumber() const { return container_->isNumber(); }
bool Value::isArray() const { return container_->isArray(); }
bool Value::isObject() const { return container_->isObject(); }
bool Value::isNull() const { return container_->isNull(); }
bool Value::isBoolean() const { return container_->isBoolean(); }

Type Value::type() const { return container_->type(); }

Value Value::operator[](std::string const& name) const {
    auto i = object().find(name);
    return (i == object().end() ? Value() : i->second);
}

Value Value::operator[](size_t index) const {
    auto const& arr = array();
    if(index < arr.size()) {
        return arr[index];
    } else {
        return Value();
    }
}

}
