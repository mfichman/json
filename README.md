JSON
====

A simple JSON library for C++.

Build Dependencies
------------------

C++11 or better and [Scons](http://www.scons.org).

Building
--------

This library uses the Scons build system, which requires Python. Install Python, then:

    easy_install scons
    git clone git@github.com:mfichman/json.git
    cd json
    scons

Examples
--------

Load an object from a string:

```
#include <json/json.hpp>

int main() {
    json::Value value = json::loads("{\"hello world!\": true});
    return 0;
}
```

Load an object from a file:

```
std::istream in("file.json");
json::Value value = json::load(in);
```

Dump an object to a string:

```
json::Value value = json::Value{ {"hello world", true} };
std::string str = json::dumps(value);
```

Dump an object to a file:

```
json::Value value = json::Value{ {"hello world", true} };
std::ostream out("file.json");
json::dump(value, out);
```

Manipulating `json::Value`s:

```
json::Value value;
value['key'] = "the quick brown fox";
value['key'] = 1;
value['key'] = true;
value['key'] = json::Value{ {"this is an object", true}, {"key2", "value2"} };
value['key'] = json::Value{ "this is an array", true };
value['key'] = nullptr;

json::String str = value['key'].string();
// typedef'd to std:;string
json::Number num = value['key'].number(); 
// typedef'd double
json::Boolean boolean = value['key'].boolean();
// typedef'd to boolean
json::Array array = value['key'].array(); 
// typedef'd to std::vector<json::Value> 
json::Object array = value['key'].array(); 
// typedef'd to std::unordered_map<std::string, Json::Value>

value.isNull(); // check if null
value.isString(); // etc.
value.isBoolean(); 
value.isNumber(); 
value.isObject(); 
value.isArray(); 
```

