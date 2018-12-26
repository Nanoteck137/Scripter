#pragma once

namespace v8 {
    class Platform;
}

#include <memory>
#include <unordered_map>

#include <v8.h>

class Engine 
{
private:
    static std::unique_ptr<v8::Platform> s_Platform;
private:
    v8::Isolate* m_Isolate;
    v8::Isolate::CreateParams m_IsolateCreateParams;
public:
    Engine();
    ~Engine();

    void StartIsolate();
    void EndIsolate();

    void ThrowException(const std::string& name);

    v8::Isolate* GetIsolate() const { return m_Isolate; }
public:
    static void InitalizeV8(const char* execPath);
    static void DeinitalizeV8();
};