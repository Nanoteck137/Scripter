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

#include "scripter/Common.h"

#include <memory>
#include <unordered_map>

#include <libplatform/libplatform.h>
#include <v8.h>

namespace scripter {

    /**
     * Engine
     *
     * Wrapper around V8's isolate object but its has some helpers too.
     */
    class Engine
    {
    private:
        static std::unique_ptr<v8::Platform> s_Platform;

    private:
        v8::Isolate* m_Isolate;
        v8::Isolate::CreateParams m_IsolateCreateParams;

    public:
        Engine();
        ~Engine();

        /**
         * Starts this instance of the isolate
         */
        void StartIsolate();

        /**
         * Ends this instance of the isolate
         */
        void EndIsolate();

        /**
         * Wrapper around ThrowException in the isolate
         */
        void ThrowException(const char* format, ...);

        /**
         * Checks a try catch if a exception was thrown, returns true if a
         * exception was thrown
         */
        bool CheckTryCatch(v8::TryCatch* tryCatch);

        /**
         * Prints an object and its properties
         */
        void PrintObject(v8::Local<v8::Context> context,
                         v8::Local<v8::Object> object);

        /**
         * Prints a value
         */
        void PrintValue(v8::Local<v8::Value> value);

        /**
         * A wrapper to create a javascript string
         */
        v8::Local<v8::String> CreateString(const std::string& value);

        /**
         * A wrapper to create a javascript string
         */
        v8::Local<v8::String> CreateString(const char* value);

        String ConvertValueToString(v8::Local<v8::Value> value);

        /**
         * Returns the V8 isolate
         */
        v8::Isolate* GetIsolate() const { return m_Isolate; }

    public:
        /**
         * Initializes the V8 library and some other systems ex. logger
         */
        static void InitializeV8(const char* execPath);

        /**
         * Deinitializes the V8 library and some other systems
         */
        static void DeinitializeV8();
    };

} // namespace scripter