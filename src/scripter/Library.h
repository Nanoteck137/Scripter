/*
 * MIT License
 *
 * Copyright (c) 2018 Nanoteck137
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <unordered_map>

#include <v8.h>
#include "Engine.h"

enum LibraryValueType
{
    LIB_VALUE_TYPE_INT32,
};

enum LibraryValueAttribute
{
    LIB_VALUE_ATTRIBUTE_READ_WRITE,
    LIB_VALUE_ATTRIBUTE_READ_ONLY,
};

struct LibraryValue
{
public:
    LibraryValueType type;
    LibraryValueAttribute attribute;
    union 
    {
        int32_t integer32;
    };
};

class Library 
{
private:
    std::string m_Name;
    std::unordered_map<std::string, v8::FunctionCallback> m_Functions;
    std::unordered_map<std::string, LibraryValue> m_Values;
public:
    Library(const std::string& name);
    ~Library();

    void RegisterFunction(const std::string& name, v8::FunctionCallback function);
    void RegisterValueInt32(const std::string& name, int32_t value, LibraryValueAttribute attribute = LIB_VALUE_ATTRIBUTE_READ_WRITE);

    v8::Local<v8::ObjectTemplate> GenerateObject(Engine* engine);

    inline std::string GetName() const { return m_Name; }
private:
    v8::PropertyAttribute ValueAttributeToV8(LibraryValueAttribute attribute);
};
