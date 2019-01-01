/*
 * MIT License
 *
 * Copyright (c) 2018 Patrik M. Rosenström
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

#include "scripter/Logger.h"
#include "scripter/NativeModuleImporter.h"

#include <v8-internal.h>

namespace scripter {

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
        m_Isolate->ThrowException(CreateString(buffer));
    }

    bool Engine::CheckTryCatch(v8::TryCatch* tryCatch)
    {
        SCRIPTER_ASSERT(tryCatch);

        if (tryCatch->HasCaught())
        {
            v8::Local<v8::Value> ex = tryCatch->Exception();
            v8::Local<v8::Message> message = tryCatch->Message();
            v8::Local<v8::StackTrace> stackTrace = message->GetStackTrace();

            SCRIPTER_LOG_ERROR(
                "------------ JAVASCRIPT EXCEPTION ------------");

            if (stackTrace->GetFrameCount() > 1)
            {
                String functionName = ConvertValueToString(
                    stackTrace->GetFrame(m_Isolate, 0)->GetFunctionName());
                if (functionName != "")
                {
                    SCRIPTER_LOG_ERROR("Function - {0}", functionName);
                }
            }

            SCRIPTER_LOG_ERROR(
                "File - {0}:{1}:{2}",
                ConvertValueToString(message->GetScriptResourceName()),
                message->GetLineNumber(m_Isolate->GetCurrentContext())
                    .ToChecked(),
                message->GetStartColumn(m_Isolate->GetCurrentContext())
                    .ToChecked());

            SCRIPTER_LOG_ERROR(
                "Code - {0}",
                ConvertValueToString(
                    message->GetSourceLine(m_Isolate->GetCurrentContext())
                        .ToLocalChecked()));

            SCRIPTER_LOG_ERROR("Message - {0}", ConvertValueToString(ex));

            SCRIPTER_LOG_ERROR("-- Stacktrace --");
            for (int i = 0; i < stackTrace->GetFrameCount(); i++)
            {
                v8::Local<v8::StackFrame> frame =
                    stackTrace->GetFrame(m_Isolate, i);

                std::string function =
                    ConvertValueToString(frame->GetFunctionName());
                std::string scriptName =
                    ConvertValueToString(frame->GetScriptName());
                int lineNumber = frame->GetLineNumber();
                int columnNumber = frame->GetColumn();
                if (function == "")
                {
                    SCRIPTER_LOG_ERROR("\tat {0}:{1}:{2}", scriptName,
                                       lineNumber, columnNumber);
                }
                else
                {
                    SCRIPTER_LOG_ERROR("\tat {0} ({1}:{2}:{3})", function,
                                       scriptName, lineNumber, columnNumber);
                }
            }

            SCRIPTER_LOG_ERROR(
                "----------------------------------------------");

            return true;
        }
        return false;
    }

    void Engine::PrintObject(v8::Local<v8::Context> context,
                             v8::Local<v8::Object> object)
    {
        SCRIPTER_LOG_INFO("------ OBJECT ------");
        v8::String::Utf8Value objectStr(m_Isolate, object);
        SCRIPTER_LOG_INFO("{0}", *objectStr);

        v8::Local<v8::Array> properties = object->GetPropertyNames();
        int length = properties->Length();

        SCRIPTER_LOG_INFO("Number of properties = {0}:", length);

        for (int i = 0; i < length; i++)
        {
            v8::Local<v8::Value> key =
                properties->Get(context, i).ToLocalChecked();
            v8::String::Utf8Value str(m_Isolate, key);

            SCRIPTER_LOG_INFO("  {0}. {1}", (i + 1), *str);
        }

        SCRIPTER_LOG_INFO("--------------------");
    }

    void Engine::PrintValue(v8::Local<v8::Value> value)
    {
        SCRIPTER_LOG_INFO("---- VALUE ----");

        v8::String::Utf8Value str(m_Isolate, value);
        SCRIPTER_LOG_INFO("{0}", *str);

        SCRIPTER_LOG_INFO("-----------\n");
    }

    v8::Local<v8::String> Engine::CreateString(const String& value)
    {
        return CreateString(value.c_str());
    }

    v8::Local<v8::String> Engine::CreateString(const char* value)
    {
        return v8::String::NewFromUtf8(m_Isolate, value,
                                       v8::NewStringType::kNormal)
            .ToLocalChecked();
    }

    String Engine::ConvertValueToString(v8::Local<v8::Value> string)
    {
        v8::String::Utf8Value value(m_Isolate, string);
        return String(*value);
    }

    void Engine::InitializeV8(const char* execPath)
    {
        // Initialize V8.
        v8::V8::InitializeICUDefaultLocation(execPath);
        v8::V8::InitializeExternalStartupData(execPath);

        s_Platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(s_Platform.get());
        v8::V8::Initialize();

        // Initialize NativeModuleImporter
        NativeModuleImporter::Initialize();

        // Initialize Logger
        Logger::Initialize();
    }

    void Engine::DeinitializeV8()
    {
        // Deinitalize V8
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();

        // Deinitalize NativeModuleImporter
        NativeModuleImporter::Deinitialize();

        // Deinitialize Logger
        Logger::Deinitialize();
    }

} // namespace scripter