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
#include "NativeModuleImporter.h"

#include <dlfcn.h>

namespace scripter {

    NativeModuleImporter* NativeModuleImporter::s_Instance;

    NativeModuleImporter::NativeModuleImporter()
    {
        m_SearchPaths = {"./bin/Debug", ".", ""};
    }

    NativeModuleImporter::~NativeModuleImporter()
    {
        for (auto it = m_Handles.begin(); it != m_Handles.end(); it++)
        {
            printf("Unloading native module: %s\n", it->first.c_str());

            dlclose(it->second);
            it->second = nullptr;
        }
    }

    Module* NativeModuleImporter::ImportModule(Engine* engine,
                                               const std::string& moduleName)
    {
        void* handle = nullptr;
        bool exists = false;

        if (m_Handles.find(moduleName) != m_Handles.end())
        {
            handle = m_Handles[moduleName];
            exists = true;
        }
        else
        {
            for (const std::string& path : m_SearchPaths)
            {
                std::string modulePath = path;

                if (modulePath != "" &&
                    modulePath[modulePath.length() - 1] != '/')
                {
                    modulePath.append(1, '/');
                }

                modulePath.append("lib");
                modulePath.append(moduleName);
                modulePath.append(".so");

                // TODO(patrik): Add a check here if the modulePath exists

                handle = dlopen(modulePath.c_str(), RTLD_NOW);
                if (handle)
                {
                    printf("Loaded native module: %s\n", moduleName.c_str());
                    exists = false;
                    break;
                }
            }
        }

        if (handle)
        {
            CreateModuleFunc func =
                (CreateModuleFunc)dlsym(handle, "CreateModule");

            if (func)
            {
                if (!exists)
                    m_Handles[moduleName] = handle;

                return func(engine);
            }
        }

        return nullptr;
    }

    NativeModuleImporter* NativeModuleImporter::Get() { return s_Instance; }

    void NativeModuleImporter::Initialize()
    {
        s_Instance = new NativeModuleImporter();
    }

    void NativeModuleImporter::Deinitialize()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

} // namespace scripter