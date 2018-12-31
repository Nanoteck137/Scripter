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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace scripter { namespace modules {

    JSFUNC(open)
    {
        JS_FUNC_ISOLATE_ENGINE();

        v8::HandleScope handleScope(isolate);

        JS_CHECK_ARGS_LENGTH(2);

        JS_CHECK_ARG(JS_TYPE_STRING, 0);
        JS_CHECK_ARG(JS_TYPE_INT32, 1);

        v8::Local<v8::String> str =
            args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

        std::string file = engine->ConvertValueToString(str);
        int flags =
            args[1]->Int32Value(isolate->GetCurrentContext()).ToChecked();

        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int result = open(file.c_str(), flags, mode);
        if (result == -1)
        {
            printf("Error open: %s\n", strerror(errno));
        }

        args.GetReturnValue().Set(result);
    }

    JSFUNC(write)
    {
        JS_FUNC_ISOLATE_ENGINE();

        v8::HandleScope handleScope(isolate);

        JS_CHECK_ARGS_LENGTH(2);

        JS_CHECK_ARG(JS_TYPE_INT32, 0);
        JS_CHECK_ARG(JS_TYPE_STRING, 1);

        int fd = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
        v8::Local<v8::String> str =
            args[1]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

        std::string content = engine->ConvertValueToString(str);
        int result = write(fd, content.c_str(), content.length());

        if (result == -1)
        {
            engine->ThrowException("File Error: %s", strerror(errno));
        }

        args.GetReturnValue().Set(result);
    }

    JSFUNC(close)
    {
        JS_FUNC_ISOLATE_ENGINE();

        v8::HandleScope handleScope(isolate);

        JS_CHECK_ARGS_LENGTH(1);
        JS_CHECK_ARG(JS_TYPE_INT32, 0);

        int fd = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
        close(fd);
    }

    System::System(Engine* engine) : Module(engine)
    {
        // TODO(patrik): Need to add the file attributes like FILE_READ
        // FILE_WRITE

        m_Functions["open"] = JSFunc_open;
        m_Functions["write"] = JSFunc_write;
        m_Functions["close"] = JSFunc_close;
    }

    System::~System() {}

    std::string System::GetPackageName() { return "system"; }

}} // namespace scripter::modules