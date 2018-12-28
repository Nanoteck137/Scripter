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
#include "Module.h"

typedef void V8JSFunction(const v8::FunctionCallbackInfo<v8::Value>&);

Module::Module(Engine* engine) : m_Engine(engine) {}
Module::~Module() {}

v8::Local<v8::ObjectTemplate> Module::GenerateObject()
{
    v8::Isolate* isolate = m_Engine->GetIsolate();
    v8::EscapableHandleScope handleScope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    for (auto it = m_Functions.begin(); it != m_Functions.end(); it++)
    {
        // V8JSFunction* callback = it->second.target<V8JSFunction>();
        result->Set(isolate, it->first.c_str(),
                    v8::FunctionTemplate::New(isolate, it->second));
    }

    return handleScope.Escape(result);
}
