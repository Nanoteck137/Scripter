/*
 * MIT License
 *
 * Copyright (c) 2019 Patrik M. Rosenström
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
#include "scripter/JavascriptModule.h"

namespace scripter {

    JavascriptModule::JavascriptModule(
        Engine* engine, const String& name,
        v8::Persistent<v8::ObjectTemplate> objTemplate)
        : Module(engine), m_Name(name)
    {
        m_ObjectTemplate = objTemplate;
    }

    JavascriptModule::~JavascriptModule() { m_ObjectTemplate.Reset(); }

    v8::Local<v8::ObjectTemplate> JavascriptModule::GenerateObject()
    {
        v8::EscapableHandleScope handleScope(m_Engine->GetIsolate());

        return handleScope.Escape(m_ObjectTemplate.Get(m_Engine->GetIsolate()));
    }

    String JavascriptModule::GetPackageName() { return m_Name; }

} // namespace scripter