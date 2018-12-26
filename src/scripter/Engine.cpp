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