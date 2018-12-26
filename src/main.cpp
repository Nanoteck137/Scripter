#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <scripter/Engine.h>
#include <scripter/Script.h>
#include <scripter/Library.h>

/* TODO(patrik):
    System Library - File, Time?, SystemInfo?
*/

#define JSFUNC(name) void JSLib_##name(const v8::FunctionCallbackInfo<v8::Value>& args)

void JSPrint(const v8::FunctionCallbackInfo<v8::Value>& args, bool newline)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);

    if(args.Length() > 0) {
        std::string result;

        for(int i = 0; i < args.Length(); i++) 
        {
            v8::String::Utf8Value str(isolate, args[i]);
            result.append(*str);

            if(i != args.Length() - 1)
                result.append(1, ' ');
        }

        if(newline)
            result.append(1, '\n');

        printf("%s", result.c_str());
    } else {
        if(newline)
            printf("\n");
    }
}

JSFUNC(print)
{
    JSPrint(args, false);
}

JSFUNC(println)
{
    JSPrint(args, true);
}

JSFUNC(open)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    Engine* engine = (Engine*)isolate->GetData(0);

    v8::HandleScope handleScope(isolate);

    if(args.Length() == 2) 
    {
        if(!args[0]->IsString()) 
        {
            engine->ThrowException("open: Argument 1 needs to be a string");
            return;
        }
        
        if(!args[1]->IsInt32()) 
        {
            engine->ThrowException("open: Argument 2 needs to be a int32");
            return;
        }

        v8::Local<v8::String> str = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();
        v8::String::Utf8Value strValue(isolate, str);
        
        const char* file = *strValue;
        int flags = args[1]->Int32Value(isolate->GetCurrentContext()).ToChecked();

        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int result = open(file, flags, mode);
        if(result == -1) 
        {
            printf("Error open: %s\n", strerror(errno));
        }

        args.GetReturnValue().Set(result);
    }
    else 
    {
        engine->ThrowException("open: Needs two arguments");
        return;
    }
}

JSFUNC(write) 
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    Engine* engine = (Engine*)isolate->GetData(0);

    v8::HandleScope handleScope(isolate);

    if(args.Length() == 2)
    {
        if(!args[0]->IsInt32()) 
        {
            engine->ThrowException("write: Argument 1 needs to be a int32");
            return;
        }

        if(!args[1]->IsString()) 
        {
            engine->ThrowException("write: Argument 2 needs to be a string");
            return;
        }

        int fd = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
        v8::Local<v8::String> str = args[1]->ToString(isolate->GetCurrentContext()).ToLocalChecked();
        v8::String::Utf8Value strValue(isolate, str);

        const char* buffer = *strValue;
        int result = write(fd, buffer, strValue.length());
        
        if(result == -1)
        {
            printf("Error: %s\n", strerror(errno));
        }

        args.GetReturnValue().Set(result);
    }
    else
    {
        engine->ThrowException("write: Needs two arguments");
        return;
    }
}

JSFUNC(close)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    Engine* engine = (Engine*)isolate->GetData(0);

    v8::HandleScope handleScope(isolate);

    if(args.Length() == 1) 
    {
        if(!args[0]->IsInt32())
        {
            engine->ThrowException("close: Argument 1 needs to be an int32");
            return;
        }

        int fd = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
        close(fd);
    }
    else 
    {
        engine->ThrowException("close: Needs one argument");
        return;
    }
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

v8::MaybeLocal<v8::Module> ModuleResolveCallback(
    v8::Local<v8::Context> context, 
    v8::Local<v8::String> specifier, 
    v8::Local<v8::Module> referrer)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::String::Utf8Value strValue(v8::Isolate::GetCurrent(), specifier);
    printf("ModuleResolveCallback %s\n", *strValue);

    std::string moduleSource = ReadFile(*strValue);

    v8::Local<v8::String> name = v8::String::NewFromUtf8(isolate, "test.js");
    v8::ScriptOrigin origin(
        name,
        v8::Integer::New(isolate, 0),
        v8::Integer::New(isolate, 0),
        v8::False(isolate),
        v8::Local<v8::Integer>(),
        v8::Local<v8::Value>(),
        v8::False(isolate),
        v8::False(isolate),
        v8::True(isolate));


    v8::ScriptCompiler::Source source(v8::String::NewFromUtf8(isolate, moduleSource.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), origin);
    v8::MaybeLocal<v8::Module> module = v8::ScriptCompiler::CompileModule(isolate, &source).ToLocalChecked();

    return module;
}

int main(int argc, const char** argv)
{
    Engine::InitalizeV8(argv[0]);

    Engine* engine = new Engine();

    v8::Isolate* isolate = engine->GetIsolate();
    engine->StartIsolate();
    {
        v8::HandleScope handleScope(isolate);

        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

        Library systemLib("system");
        systemLib.RegisterFunction("print", JSLib_print);
        systemLib.RegisterFunction("println", JSLib_println);

        systemLib.RegisterValueInt32("FILE_READ_ONLY", O_RDONLY, LIB_VALUE_ATTRIBUTE_READ_ONLY);
        systemLib.RegisterValueInt32("FILE_WRITE_ONLY", O_WRONLY, LIB_VALUE_ATTRIBUTE_READ_ONLY);
        systemLib.RegisterValueInt32("FILE_READ_WRITE", O_RDWR, LIB_VALUE_ATTRIBUTE_READ_ONLY);
        systemLib.RegisterValueInt32("FILE_CREATE", O_CREAT, LIB_VALUE_ATTRIBUTE_READ_ONLY);
        systemLib.RegisterValueInt32("FILE_TRUNCATE", O_TRUNC, LIB_VALUE_ATTRIBUTE_READ_ONLY);

        systemLib.RegisterFunction("open", JSLib_open);
        systemLib.RegisterFunction("write", JSLib_write);
        systemLib.RegisterFunction("close", JSLib_close);

        global->Set(isolate, systemLib.GetName().c_str(), systemLib.GenerateObject(engine));

        Script script(engine, global);
        script.Enable();

        std::string moduleSource = ReadFile("test.js");

        v8::TryCatch tryCatch(isolate);

        v8::Local<v8::String> name = v8::String::NewFromUtf8(isolate, "test.js");
        v8::ScriptOrigin origin(
            name,
            v8::Integer::New(isolate, 0),
            v8::Integer::New(isolate, 0),
            v8::False(isolate),
            v8::Local<v8::Integer>(),
            v8::Local<v8::Value>(),
            v8::False(isolate),
            v8::False(isolate),
            v8::True(isolate));


        v8::ScriptCompiler::Source source(v8::String::NewFromUtf8(isolate, moduleSource.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), origin);
        v8::MaybeLocal<v8::Module> mayModule = v8::ScriptCompiler::CompileModule(isolate, &source);
        if(tryCatch.HasCaught()) 
        {
            v8::Local<v8::Value> ex = tryCatch.Exception();

            v8::String::Utf8Value str(isolate, ex);

            printf("Exception %s\n", *str);
        } 
        else 
        {
            v8::Local<v8::Module> module = mayModule.ToLocalChecked();
            v8::Module::Status status = module->GetStatus();

            v8::Maybe<bool> res = module->InstantiateModule(script.GetContext(), ModuleResolveCallback);

            module->Evaluate(script.GetContext()).ToLocalChecked();
            status = module->GetStatus();
        }

        std::string jsSource = ReadFile("test.js");

        //script.CompileAndRun(jsSource);

        v8::Local<v8::Function> func = script.GetFunction("main").ToLocalChecked();
        func->Call(script.GetContext(), v8::Null(isolate), 0, {});

        script.Disable();
    }

    engine->EndIsolate();

    delete engine;
    Engine::DeinitalizeV8();

    return 0;
}
