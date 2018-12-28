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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>

#include <scripter/Engine.h>
#include <scripter/Script.h>

#include <scripter/modules/System.h>

/* TODO(patrik):
    System Library - File, Time?, SystemInfo?
*/

JSFUNC(open)
{
    JS_FUNC_START();

    v8::HandleScope handleScope(isolate);

    JS_CHECK_ARGS_LENGTH(2);

    JS_CHECK_ARG(JS_TYPE_STRING, 0);
    JS_CHECK_ARG(JS_TYPE_INT32, 1);

    v8::Local<v8::String> str =
        args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();
    v8::String::Utf8Value strValue(isolate, str);

    const char* file = *strValue;
    int flags = args[1]->Int32Value(isolate->GetCurrentContext()).ToChecked();

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int result = open(file, flags, mode);
    if (result == -1)
    {
        printf("Error open: %s\n", strerror(errno));
    }

    args.GetReturnValue().Set(result);
}

JSFUNC(write)
{
    JS_FUNC_START();

    v8::HandleScope handleScope(isolate);

    JS_CHECK_ARGS_LENGTH(2);

    JS_CHECK_ARG(JS_TYPE_INT32, 0);
    JS_CHECK_ARG(JS_TYPE_STRING, 1);

    int fd = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
    v8::Local<v8::String> str =
        args[1]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    v8::String::Utf8Value strValue(isolate, str);

    const char* buffer = *strValue;
    int result = write(fd, buffer, strValue.length());

    if (result == -1)
    {
        engine->ThrowException("File Error: %s", strerror(errno));
    }

    args.GetReturnValue().Set(result);
}

JSFUNC(close)
{
    JS_FUNC_START();

    v8::HandleScope handleScope(isolate);

    JS_CHECK_ARGS_LENGTH(1);
    JS_CHECK_ARG(JS_TYPE_INT32, 0);

    int fd = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
    close(fd);
}

std::string ReadFile(const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "rt");

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::string result(length, 0);
    fread(&result[0], 1, length, file);

    fclose(file);

    return result;
}

using namespace scripter;

int main(int argc, const char** argv)
{
    Engine::InitalizeV8(argv[0]);

    Engine* engine = new Engine();

    v8::Isolate* isolate = engine->GetIsolate();

    engine->StartIsolate();
    {
        v8::HandleScope handleScope(isolate);

        v8::TryCatch tryCatch(isolate);

        modules::System* systemModule = new modules::System(engine);

        Module* modules[] = {systemModule};

        Script script(engine, modules, 1);
        script.Enable();

        std::string scriptSource = ReadFile("scripts/test.js");
        script.CompileAndRun(scriptSource);

        auto function = script.GetFunction("main").ToLocalChecked();

        char fullScriptPath[PATH_MAX] = {};
        realpath("script/test.js", fullScriptPath);
        int length = strlen(fullScriptPath);
        strcpy(fullScriptPath + length, "/test.js");

        v8::Local<v8::Value> args[] = {
            v8::String::NewFromUtf8(isolate, fullScriptPath,
                                    v8::NewStringType::kNormal)
                .ToLocalChecked(),
        };

        v8::Local<v8::Array> array = v8::Array::New(isolate, args, 1);
        v8::Local<v8::Value> funcArgs[] = {array};

        function->Call(v8::Null(isolate), 1, funcArgs);

        script.Disable();
    }

    engine->EndIsolate();

    delete engine;
    Engine::DeinitalizeV8();

    return 0;
}
