#pragma once

#include <v8.h>

class Engine;

class Script 
{
private:
    Engine* m_Engine;
    v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>> m_Context;
public:
    Script(Engine* engine, const v8::Local<v8::ObjectTemplate>& globals);
    ~Script();

    void Enable();
    void Disable();

    v8::MaybeLocal<v8::Value> CompileAndRun(const std::string& code);
    v8::Local<v8::Context> GetContext();

    v8::MaybeLocal<v8::Function> GetFunction(const std::string& name);
};