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
#pragma once

#include <v8.h>

#include "Engine.h"
#include "Module.h"

namespace scripter {

    class Script
    {
    private:
        Engine* m_Engine;
        v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>>
            m_Context;

    public:
        Script(Engine* engine, Module* modules[], uint32_t moduleCount);
        ~Script();

        void Enable();
        void Disable();

        v8::MaybeLocal<v8::Value> CompileAndRun(const std::string& code);
        v8::Local<v8::Context> GetContext();

        v8::MaybeLocal<v8::Function> GetFunction(const std::string& name);
    };

} // namespace scripter