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

#pragma once

namespace v8 {
    class Platform;
} // namespace v8

#include <memory>
#include <unordered_map>

#include <v8.h>

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

    void StartIsolate();
    void EndIsolate();

    void ThrowException(const char* format, ...);

    void PrintObject(v8::Local<v8::Context> context,
                     v8::Local<v8::Object> object);
    void PrintValue(v8::Local<v8::Value> value);

    v8::Isolate* GetIsolate() const { return m_Isolate; }

public:
    static void InitalizeV8(const char* execPath);
    static void DeinitalizeV8();
};