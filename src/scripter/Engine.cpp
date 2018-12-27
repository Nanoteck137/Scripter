/*
 * MIT License
 *
 * Copyright (c) 2018 Nanoteck137
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Engine.h"

#include <libplatform/libplatform.h>
#include <v8.h>

std::unique_ptr<v8::Platform> Engine::s_Platform;

Engine::Engine()
{
    m_IsolateCreateParams.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    m_Isolate = v8::Isolate::New(m_IsolateCreateParams);

    m_Isolate->SetData(0, this);
}

Engine::~Engine()
{
    m_Isolate->Dispose();
    delete m_IsolateCreateParams.array_buffer_allocator;
}

void Engine::StartIsolate() { m_Isolate->Enter(); }

void Engine::EndIsolate() { m_Isolate->Exit(); }

void Engine::ThrowException(const char* format, ...)
{
    char buffer[1024] = {};

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    v8::HandleScope handleScope(m_Isolate);
    m_Isolate->ThrowException(
        v8::String::NewFromUtf8(m_Isolate, buffer, v8::NewStringType::kNormal)
            .ToLocalChecked());
}

void Engine::PrintObject(v8::Local<v8::Context> context,
                         v8::Local<v8::Object> object)
{
    printf("-- OBJECT --\n");
    v8::String::Utf8Value objectStr(m_Isolate, object);
    printf("%s\n", *objectStr);

    v8::Local<v8::Array> properties = object->GetPropertyNames();
    int length = properties->Length();
    printf("Number of properties = %d:\n", length);
    for (int i = 0; i < length; i++)
    {
        v8::Local<v8::Value> key = properties->Get(context, i).ToLocalChecked();
        v8::String::Utf8Value str(m_Isolate, key);

        printf("\t%d. %s\n", (i + 1), *str);
    }

    printf("------------\n");
}

void Engine::PrintValue(v8::Local<v8::Value> value)
{
    printf("-- VALUE --\n");

    v8::String::Utf8Value str(m_Isolate, value);
    printf("%s\n", *str);

    printf("-----------\n");
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