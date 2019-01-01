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
#include "Console.h"

#include "../Logger.h"

namespace scripter { namespace modules {

    JSFUNC(info)
    {
        JS_FUNC_ISOLATE();
        v8::HandleScope handleScope(isolate);

        String result;

        for (int32 i = 0; i < args.Length(); i++)
        {
            v8::String::Utf8Value str(isolate, args[i]);
            result.append(*str);

            if (i != args.Length() - 1)
                result.append(1, ' ');
        }

        JS_LOG_INFO("{0}", result);
    }

    JSFUNC(warning)
    {
        JS_FUNC_ISOLATE();
        v8::HandleScope handleScope(isolate);

        String result;

        for (int32 i = 0; i < args.Length(); i++)
        {
            v8::String::Utf8Value str(isolate, args[i]);
            result.append(*str);

            if (i != args.Length() - 1)
                result.append(1, ' ');
        }

        JS_LOG_WARNING("{0}", result);
    }

    JSFUNC(error)
    {
        JS_FUNC_ISOLATE();
        v8::HandleScope handleScope(isolate);

        String result;

        for (int32 i = 0; i < args.Length(); i++)
        {
            v8::String::Utf8Value str(isolate, args[i]);
            result.append(*str);

            if (i != args.Length() - 1)
                result.append(1, ' ');
        }

        JS_LOG_ERROR("{0}", result);
    }

    JSFUNC(critical)
    {
        JS_FUNC_ISOLATE();
        v8::HandleScope handleScope(isolate);

        String result;

        for (int32 i = 0; i < args.Length(); i++)
        {
            v8::String::Utf8Value str(isolate, args[i]);
            result.append(*str);

            if (i != args.Length() - 1)
                result.append(1, ' ');
        }

        JS_LOG_CRITICAL("{0}", result);
    }

    JSFUNC(assert)
    {
        JS_FUNC_ISOLATE_ENGINE();
        v8::HandleScope handleScope(isolate);

        JS_CHECK_ARG(JS_TYPE_BOOLEAN, 0);

        bool assertCondition = args[0]->BooleanValue(isolate);
        if (!assertCondition)
        {
            String result;
            for (int32 i = 1; i < args.Length(); i++)
            {
                v8::String::Utf8Value str(isolate, args[i]);
                result.append(*str);

                if (i != args.Length() - 1)
                    result.append(1, ' ');
            }

            SCRIPTER_ASSERT(false, "{0}", result);
        }
    }

    Console::Console(Engine* engine) : Module(engine)
    {
        m_Functions["info"] = JSFunc_info;
        m_Functions["warning"] = JSFunc_warning;
        m_Functions["error"] = JSFunc_error;
        m_Functions["critical"] = JSFunc_critical;
        m_Functions["assert"] = JSFunc_assert;
    }

    Console::~Console() {}

    String Console::GetPackageName() { return "console"; }

}} // namespace scripter::modules
