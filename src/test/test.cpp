#include <stdio.h>

#include <scripter/Module.h>
#include <scripter/Logger.h>

JSFUNC(test) { MODULE_LOG_INFO("Hello World from module"); }

using namespace scripter;

class TestModule : public Module
{
public:
    TestModule(Engine* engine) : Module(engine)
    {
        m_Functions["wow"] = JSFunc_test;
    }

    ~TestModule() {}

    virtual std::string GetPackageName() override { return "test"; }
};

extern "C"
{
    Module* CreateModule(Engine* engine) { return new TestModule(engine); }
}