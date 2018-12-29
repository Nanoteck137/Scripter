#include "NativeModuleImporter.h"

#include <dlfcn.h>

namespace scripter {

    NativeModuleImporter* NativeModuleImporter::s_Instance;

    NativeModuleImporter::NativeModuleImporter()
    {
        m_SearchPaths = {"./bin/Debug", ""};
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

    void NativeModuleImporter::Initalize()
    {
        s_Instance = new NativeModuleImporter();
    }

    void NativeModuleImporter::Deinitalize()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

} // namespace scripter