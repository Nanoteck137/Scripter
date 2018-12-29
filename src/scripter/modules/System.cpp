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
#include "System.h"

void JSPrint(const v8::FunctionCallbackInfo<v8::Value>& args, bool newline)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);

    if (args.Length() > 0)
    {
        std::string result;

        for (int i = 0; i < args.Length(); i++)
        {
            v8::String::Utf8Value str(isolate, args[i]);
            result.append(*str);

            if (i != args.Length() - 1)
                result.append(1, ' ');
        }

        if (newline)
            result.append(1, '\n');

        printf("%s", result.c_str());
    }
    else
    {
        if (newline)
            printf("\n");
    }
}

JSFUNC(print) { JSPrint(args, false); }

JSFUNC(println) { JSPrint(args, true); }

namespace scripter { namespace modules {

    System::System(Engine* engine) : Module(engine)
    {
        m_Functions["print"] = JSLib_print;
        m_Functions["println"] = JSLib_println;
    }

    System::~System() {}

    std::string System::GetPackageName() { return "system"; }

}} // namespace scripter::modules