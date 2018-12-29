#include "NativeModuleImporter.h"

namespace scripter {

    NativeModuleImporter* NativeModuleImporter::s_Instance;

    NativeModuleImporter::NativeModuleImporter() {}
    NativeModuleImporter::~NativeModuleImporter() {}

    Module* NativeModuleImporter::ImportModule(const std::string& modulePath)
    {
        return nullptr;
    }

    NativeModuleImporter* NativeModuleImporter::Get() { return s_Instance; }

    void NativeModuleImporter::Initalize()
    {
        s_Instance = new NativeModuleImporter();
    }

    void NativeModuleImporter::Deinitalize() { delete s_Instance; }

} // namespace scripter