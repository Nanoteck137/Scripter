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
#include "scripter/JavascriptModuleImporter.h"

#include "scripter/Logger.h"

#include "scripter/utils/File.h"
#include "scripter/utils/Path.h"

#include "scripter/ScriptEnv.h"

namespace scripter {

    JavascriptModuleImporter* JavascriptModuleImporter::s_Instance;

    JavascriptModuleImporter::JavascriptModuleImporter() {}
    JavascriptModuleImporter::~JavascriptModuleImporter() {}

    Module* JavascriptModuleImporter::ImportModule(Engine* engine,
                                                   const String& moduleName,
                                                   const String& scriptPath)
    {
        // TODO(patrik): Make this search for other places
        String directory = Path::GetDirectoryPath(scriptPath);
        String modulePath = Path::Append(directory, moduleName);
        modulePath.append(".js");

        if (File::Exists(modulePath))
        {
            return LoadModule(engine, modulePath);
        }

        return nullptr;
    }

    Module* JavascriptModuleImporter::LoadModule(Engine* engine,
                                                 const String& modulePath)
    {
        v8::HandleScope scope(engine->GetIsolate());

        ScriptEnv* env = new ScriptEnv(engine);
        env->Enable();

        env->CompileAndRun(modulePath);

        v8::Local<v8::Context> context = env->GetContext();

        env->Disable();

        // NOTE(patrik): Do we need to cache the whole enviroment??
        delete env;

        return nullptr;
    }

    JavascriptModuleImporter* JavascriptModuleImporter::Get()
    {
        return s_Instance;
    }

    void JavascriptModuleImporter::Initialize()
    {
        s_Instance = new JavascriptModuleImporter();
    }

    void JavascriptModuleImporter::Deinitialize()
    {
        if (s_Instance)
        {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

} // namespace scripter