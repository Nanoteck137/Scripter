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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>

#include <scripter/Logger.h>
#include <scripter/Engine.h>
#include <scripter/ScriptEnv.h>

#include <scripter/modules/System.h>
#include <scripter/modules/Console.h>

using namespace scripter;

int main(int argc, const char** argv)
{
    Engine::InitializeV8(argv[0]);

    Engine* engine = new Engine();

    v8::Isolate* isolate = engine->GetIsolate();

    engine->StartIsolate();
    {
        v8::HandleScope handleScope(isolate);

        v8::TryCatch tryCatch(isolate);

        modules::System* systemModule = new modules::System(engine);
        modules::Console* consoleModule = new modules::Console(engine);

        ScriptEnv env(engine);
        env.Enable();

        env.ImportModule(systemModule);
        env.ImportModule(consoleModule);

        env.CompileAndRun("tests/test.js");

        auto function = env.GetFunction("main").ToLocalChecked();

        function->Call(v8::Null(isolate), 0, {});
        engine->CheckTryCatch(&tryCatch);

        env.Disable();

        delete systemModule;
        delete consoleModule;
    }

    engine->EndIsolate();

    delete engine;
    Engine::DeinitializeV8();

    return 0;
}
