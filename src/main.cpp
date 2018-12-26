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

#define JS_CHECK_ARGS_LENGTH(x) \
    if(args.Length() != x) { \
        engine->ThrowException("%s: Needs %d argument(s)", __FUNCTION__, x); \
        return; \
    }

#define JS_TYPE_INT32 Int32
#define JS_TYPE_STRING String

#define JS_TYPE_UNDEFINED Undefined
#define JS_TYPE_NULL Null
#define JS_TYPE_NULL_OR_UNDEFINED NullOrUndefined
#define JS_TYPE_TRUE True
#define JS_TYPE_FALSE False
#define JS_TYPE_NAME Name
#define JS_TYPE_STRING String
#define JS_TYPE_SYMBOL Symbol
#define JS_TYPE_FUNCTION Function
#define JS_TYPE_ARRAY Array
#define JS_TYPE_OBJECT Object
#define JS_TYPE_BIG_INT BigInt
#define JS_TYPE_BOOLEAN Boolean
#define JS_TYPE_NUMBER Number
#define JS_TYPE_EXTERNAL External
#define JS_TYPE_INT32 Int32
#define JS_TYPE_UINT32 Uint32
#define JS_TYPE_DATE Date
#define JS_TYPE_ARGUMENTS_OBJECT ArgumentsObject
#define JS_TYPE_BIG_INT_OBJECT BigIntObject
#define JS_TYPE_BOOLEAN_OBJECT BooleanObject
#define JS_TYPE_NUMBER_OBJECT NumberObject
#define JS_TYPE_STRING_OBJECT StringObject
#define JS_TYPE_SYMBOL_OBJECT SymbolObject
#define JS_TYPE_NATIVE_ERROR NativeError
#define JS_TYPE_REG_EXP RegExp
#define JS_TYPE_ASYNC_FUNCTION AsyncFunction
#define JS_TYPE_GENERATOR_FUNCTION GeneratorFunction
#define JS_TYPE_PROMISE Promise
#define JS_TYPE_MAP Map
#define JS_TYPE_SET Set
#define JS_TYPE_MAP_ITERATOR MapIterator
#define JS_TYPE_SET_ITERATOR SetIterator
#define JS_TYPE_WEAK_MAP WeakMap
#define JS_TYPE_WEAK_SET WeakSet
#define JS_TYPE_ARRAY_BUFFER ArrayBuffer
#define JS_TYPE_ARRAY_BUFFER_VIEW ArrayBufferView
#define JS_TYPE_TYPED_ARRAY TypedArray
#define JS_TYPE_UINT8_ARRAY Uint8Array
#define JS_TYPE_UINT8_CLAMPED_ARRAY Uint8ClampedArray
#define JS_TYPE_INT8_ARRAY Int8Array
#define JS_TYPE_UINT16_ARRAY Uint16Array
#define JS_TYPE_INT16_ARRAY Int16Array
#define JS_TYPE_UINT32_ARRAY Uint32Array
#define JS_TYPE_INT32_ARRAY Int32Array
#define JS_TYPE_FLOAT32_ARRAY Float32Array
#define JS_TYPE_FLOAT64_ARRAY Float64Array
#define JS_TYPE_BIG_INT64_ARRAY BigInt64Array
#define JS_TYPE_BIG_UINT64_ARRAY BigUint64Array
#define JS_TYPE_DATA_VIEW DataView
#define JS_TYPE_SHARED_ARRAY_BUFFER SharedArrayBuffer
#define JS_TYPE_PROXY Proxy
#define JS_TYPE_WEB_ASSEMBLY_COMPILED_MODULE WebAssemblyCompiledModule
#define JS_TYPE_MODULE_NAMESPACE_OBJECT ModuleNamespaceObject

#define _JS_CHECK_ARG(type, x) \
    if(!args[x]->Is##type()) { \
        engine->ThrowException("%s: Needs %d argument to be %s", __FUNCTION__, x, #type); \
        return; \
    }

#define JS_CHECK_ARG(type, x) _JS_CHECK_ARG(type, x)

#define JS_FUNC_START() \
    v8::Isolate* isolate = v8::Isolate::GetCurrent(); \
    Engine* engine = (Engine*)isolate->GetData(0);

JSFUNC(open)
{
    JS_FUNC_START();

    v8::HandleScope handleScope(isolate);

    JS_CHECK_ARGS_LENGTH(2);

    JS_CHECK_ARG(JS_TYPE_STRING, 0);
    JS_CHECK_ARG(JS_TYPE_INT32, 1);

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

JSFUNC(write) 
{
    JS_FUNC_START();

    v8::HandleScope handleScope(isolate);

    JS_CHECK_ARGS_LENGTH(2);

    JS_CHECK_ARG(JS_TYPE_INT32, 0);
    JS_CHECK_ARG(JS_TYPE_STRING, 1);

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

            module->Evaluate(script.GetContext());
            if(tryCatch.HasCaught())
            {
                v8::Local<v8::Value> ex = tryCatch.Exception();

                v8::String::Utf8Value str(isolate, ex);

                printf("Exception %s\n", *str);
            } 

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