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
#include "ScriptEnv.h"

#include "scripter/NativeModuleImporter.h"
#include "scripter/Logger.h"

namespace scripter {

    JSFUNC(importModule)
    {
        JS_FUNC_ISOLATE_ENGINE();
        v8::HandleScope handleScope(isolate);

        JS_CHECK_ARGS_LENGTH(1);

        JS_CHECK_ARG(JS_TYPE_STRING, 0);

        v8::String::Utf8Value moduleNameStr(isolate,
                                            args[0]->ToString(isolate));
        std::string moduleName = std::string(*moduleNameStr);

        Module* module =
            NativeModuleImporter::Get()->ImportModule(engine, moduleName);

        if (!module)
        {
            engine->ThrowException("Could not load module %s",
                                   moduleName.c_str());
            return;
        }

        v8::Local<v8::External> data =
            v8::Local<v8::External>::Cast(args.Data());
        ScriptEnv* script = (ScriptEnv*)data->Value();

        script->ImportModule(module);
    }

    ScriptEnv::ScriptEnv(Engine* engine) : m_Engine(engine)
    {
        v8::Isolate* isolate = engine->GetIsolate();
        v8::HandleScope scope(isolate);

        v8::Local<v8::ObjectTemplate> globals =
            v8::ObjectTemplate::New(isolate);

        v8::Local<v8::External> data = v8::External::New(isolate, this);

        globals->Set(
            isolate, "importModule",
            v8::FunctionTemplate::New(isolate, JSFunc_importModule, data));

        v8::Local<v8::Context> context =
            v8::Context::New(isolate, NULL, globals);
        m_Context = v8::Persistent<v8::Context,
                                   v8::CopyablePersistentTraits<v8::Context>>(
            isolate, context);
    }

    ScriptEnv::~ScriptEnv() { m_Context.Reset(); }

    void ScriptEnv::Enable() { m_Context.Get(m_Engine->GetIsolate())->Enter(); }

    void ScriptEnv::Disable() { m_Context.Get(m_Engine->GetIsolate())->Exit(); }

    void ScriptEnv::ImportModule(Module* module)
    {
        // TODO: Check if module already loaded
        v8::Local<v8::Context> context = GetContext();
        v8::Local<v8::Object> globals =
            v8::Local<v8::Object>::Cast(context->Global()->GetPrototype());

        v8::Local<v8::ObjectTemplate> objTemplate = module->GenerateObject();
        globals->Set(m_Engine->CreateString(module->GetPackageName()),
                     objTemplate->NewInstance(context).ToLocalChecked());
    }

    v8::MaybeLocal<v8::Value> ScriptEnv::CompileAndRun(const std::string& code)
    {
        v8::Isolate* isolate = m_Engine->GetIsolate();

        v8::EscapableHandleScope handleScope(isolate);
        v8::TryCatch tryCatch(isolate);

        v8::ScriptOrigin origin(
            m_Engine->CreateString("TODO"),
            v8::Local<v8::Integer>(),         // resource_line_offset
            v8::Local<v8::Integer>(),         // resource_column_offset
            v8::False(isolate),               // resource_is_shared_cross_origin
            v8::Local<v8::Integer>(),         // script_id
            v8::Local<v8::Value>(),           // source_map_url
            v8::False(isolate),               // resource_is_opaque
            v8::Local<v8::Boolean>(),         // is_wasm
            v8::Local<v8::Boolean>(),         // is_module
            v8::Local<v8::PrimitiveArray>()); // host_defined_options

        v8::ScriptCompiler::Source source(m_Engine->CreateString(code), origin);

        v8::MaybeLocal<v8::Value> result;
        v8::Local<v8::Script> script;
        if (!v8::ScriptCompiler::Compile(GetContext(), &source)
                 .ToLocal(&script))
        {
            if (tryCatch.HasCaught())
            {
                v8::Local<v8::Value> ex = tryCatch.Exception();

                v8::String::Utf8Value str(isolate, ex);

                SCRIPTER_LOG_ERROR("Exception: {0}", *str);

                return result;
            }
        }
        else
        {
            result = script->Run(m_Context.Get(isolate));
            if (tryCatch.HasCaught())
            {
                v8::Local<v8::Value> ex = tryCatch.Exception();

                v8::String::Utf8Value str(isolate, ex);

                SCRIPTER_LOG_ERROR("Exception: {0}", *str);

                return result;
            }
        }

        return handleScope.EscapeMaybe(result);
    }

    v8::Local<v8::Context> ScriptEnv::GetContext()
    {
        v8::EscapableHandleScope handleScope(m_Engine->GetIsolate());

        return handleScope.Escape(m_Context.Get(m_Engine->GetIsolate()));
    }

    v8::MaybeLocal<v8::Function> ScriptEnv::GetFunction(const std::string& name)
    {
        v8::Isolate* isolate = m_Engine->GetIsolate();

        v8::EscapableHandleScope handleScope(isolate);

        v8::Local<v8::Context> context = m_Context.Get(isolate);

        v8::MaybeLocal<v8::Value> function =
            context->Global()->Get(context, m_Engine->CreateString(name));

        if (function.IsEmpty())
        {
            printf(
                "Error - ScriptEnv::GetFunction: Could not find function '%s'\n",
                name.c_str());
            return v8::MaybeLocal<v8::Function>();
        }

        v8::Local<v8::Value> test = function.ToLocalChecked();

        v8::Local<v8::Function> result = v8::Local<v8::Function>::Cast(test);

        return handleScope.EscapeMaybe(v8::MaybeLocal<v8::Function>(result));
    }

} // namespace scripter