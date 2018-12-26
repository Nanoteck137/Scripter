#include "Library.h"

Library::Library(const std::string& name) 
    : m_Name(name)
{

}

Library::~Library() { }

void Library::RegisterFunction(const std::string& name, v8::FunctionCallback function)
{
    if(m_Functions.find(name) != m_Functions.end())
    {
        printf("Error - Library::RegisterFunction: Could not register function, there's already a function with name '%s'\n", name.c_str());
        return;
    }

    m_Functions[name] = function;
}

void Library::RegisterValueInt32(const std::string& name, int32_t value, LibraryValueAttribute attribute)
{
    if(m_Values.find(name) != m_Values.end())
    {
        printf("Error - Library::RegisterFunction: Could not register value, there's already a value with name '%s'\n", name.c_str());
        return;
    }

    LibraryValue libValue = {};
    libValue.type = LIB_VALUE_TYPE_INT32;
    libValue.integer32 = value;
    libValue.attribute = attribute;

    m_Values[name] = libValue;
}

v8::Local<v8::ObjectTemplate> Library::GenerateObject(Engine* engine) 
{
    v8::Isolate* isolate = engine->GetIsolate();
    v8::EscapableHandleScope handleScope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    for (std::unordered_map<std::string, v8::FunctionCallback>::iterator it = m_Functions.begin(); 
            it != m_Functions.end(); it++)
    {
        result->Set(isolate, it->first.c_str(), v8::FunctionTemplate::New(isolate, it->second));
    }

    for (std::unordered_map<std::string, LibraryValue>::iterator it = m_Values.begin(); 
            it != m_Values.end(); it++)
    {
        v8::Local<v8::String> name = v8::String::NewFromUtf8(isolate, it->first.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
        switch(it->second.type)
        {
            case LIB_VALUE_TYPE_INT32:
                result->Set(name, v8::Int32::New(isolate, it->second.integer32), ValueAttributeToV8(it->second.attribute));
                break;
            default:
                printf("Error - Library::GenerateObject: Unknown value type '%s'\n", it->first.c_str());
                break;
        }
    }

    return handleScope.Escape(result);
}

v8::PropertyAttribute Library::ValueAttributeToV8(LibraryValueAttribute attribute) 
{
    switch(attribute)
    {
        case LIB_VALUE_ATTRIBUTE_READ_WRITE: return v8::PropertyAttribute::None;
        case LIB_VALUE_ATTRIBUTE_READ_ONLY: return v8::PropertyAttribute::ReadOnly;
        default:
            printf("Error - Library::ValueAttributeToV8: Unknown attribute type\n");
            return v8::PropertyAttribute::None;
    }
}