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
