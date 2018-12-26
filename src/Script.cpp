#include "Script.h"

#include "Engine.h"

Script::Script(Engine* engine, const v8::Local<v8::ObjectTemplate>& globals)
    : m_Engine(engine)
{
    v8::Local<v8::Context> context = v8::Context::New(engine->GetIsolate(), NULL, globals);
    m_Context = v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>>(engine->GetIsolate(), context);
}

Script::~Script()
{
    m_Context.Reset();
}

void Script::Enable() 
{
    m_Context.Get(m_Engine->GetIsolate())->Enter();
}

void Script::Disable()
{
    m_Context.Get(m_Engine->GetIsolate())->Exit();
}

v8::MaybeLocal<v8::Value> Script::CompileAndRun(const std::string& code)
{
    v8::Isolate* isolate = m_Engine->GetIsolate();

    v8::EscapableHandleScope handleScope(isolate);
    v8::TryCatch tryCatch(isolate);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, code.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

    v8::MaybeLocal<v8::Value> result;
    v8::Local<v8::Script> script;
    if(!v8::Script::Compile(m_Context.Get(isolate), source).ToLocal(&script))
    {
        if(tryCatch.HasCaught())
        {   
            v8::Local<v8::Value> ex = tryCatch.Exception();

            v8::String::Utf8Value str(isolate, ex);

            printf("Exception %s\n", *str);
        }
    }
    else
    {
        result = script->Run(m_Context.Get(isolate));
        if(tryCatch.HasCaught())
        {   
            v8::Local<v8::Value> ex = tryCatch.Exception();

            v8::String::Utf8Value str(isolate, ex);

            printf("Exception %s\n", *str);
        }
    }

    return handleScope.EscapeMaybe(result);
}

v8::Local<v8::Context> Script::GetContext()
{
    v8::EscapableHandleScope handleScope(m_Engine->GetIsolate());

    return handleScope.Escape(m_Context.Get(m_Engine->GetIsolate()));
}

v8::MaybeLocal<v8::Function> Script::GetFunction(const std::string& name)
{
    v8::Isolate* isolate = m_Engine->GetIsolate();

    v8::EscapableHandleScope handleScope(isolate);

    v8::Local<v8::Context> context = m_Context.Get(isolate);

    v8::MaybeLocal<v8::Value> function = context->Global()->Get(
        context, 
        v8::String::NewFromUtf8(
            isolate, 
            name.c_str(), 
            v8::NewStringType::kNormal).ToLocalChecked());

    if(function.IsEmpty()) 
    {
        printf("Error - Script::GetFunction: Could not find function '%s'\n", name.c_str());
        return v8::MaybeLocal<v8::Function>();
    }

    v8::Local<v8::Function> result = v8::Local<v8::Function>::Cast(function.ToLocalChecked());

    return handleScope.EscapeMaybe(v8::MaybeLocal<v8::Function>(result));
}