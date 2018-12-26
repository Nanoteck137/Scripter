#include "Engine.h"

#include <libplatform/libplatform.h>
#include <v8.h>

std::unique_ptr<v8::Platform> Engine::s_Platform;

Engine::Engine()
{
    m_IsolateCreateParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    m_Isolate = v8::Isolate::New(m_IsolateCreateParams);

    m_Isolate->SetData(0, this);
}

Engine::~Engine()
{
    m_Isolate->Dispose();
    delete m_IsolateCreateParams.array_buffer_allocator;
}

void Engine::StartIsolate()
{
    m_Isolate->Enter();
}

void Engine::EndIsolate()
{
    m_Isolate->Exit();
}

void Engine::ThrowException(const char* format, ...)
{
    char buffer[1024] = {};

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    v8::HandleScope handleScope(m_Isolate);
    m_Isolate->ThrowException(v8::String::NewFromUtf8(m_Isolate, buffer, v8::NewStringType::kNormal).ToLocalChecked());
}

void Engine::InitalizeV8(const char* execPath)
{
    // Initialize V8.
    v8::V8::InitializeICUDefaultLocation(execPath);
    v8::V8::InitializeExternalStartupData(execPath);

    s_Platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(s_Platform.get());
    v8::V8::Initialize();
}

void Engine::DeinitalizeV8()
{
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
}