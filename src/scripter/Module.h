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
#pragma once

#include "scripter/Common.h"
#include "scripter/Engine.h"

#include <string>
#include <unordered_map>
#include <functional>

#include <v8.h>

#define JSFUNC(name)                                                           \
    void JSFunc_##name(const v8::FunctionCallbackInfo<v8::Value>& args)

#define JS_CHECK_ARGS_LENGTH(x)                                                \
    if (args.Length() < x)                                                     \
    {                                                                          \
        engine->ThrowException("%s: Needs %d or more arguments", __FUNCTION__, \
                               x);                                             \
        return;                                                                \
    }

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

#define _JS_CHECK_ARG(type, x)                                                 \
    if (!args[x]->Is##type())                                                  \
    {                                                                          \
        engine->ThrowException("%s: Needs %d argument to be %s", __FUNCTION__, \
                               x, #type);                                      \
        return;                                                                \
    }

#define JS_CHECK_ARG(type, x) _JS_CHECK_ARG(type, x)

#define JS_FUNC_ISOLATE() v8::Isolate* isolate = v8::Isolate::GetCurrent()
#define JS_FUNC_ENGINE()                                                       \
    scripter::Engine* engine = (scripter::Engine*)isolate->GetData(0)

#define JS_FUNC_ISOLATE_ENGINE()                                               \
    JS_FUNC_ISOLATE();                                                         \
    JS_FUNC_ENGINE()

namespace scripter {

    /**
     * Module
     *
     * This is a module interface to represent a javascript module, this is not
     * the same as a ES6 module or Nodejs module but its has the same purpose
     */
    class Module
    {
    public:
        friend class ScriptEnv;

    protected:
        Engine* m_Engine;
        std::unordered_map<std::string, v8::FunctionCallback> m_Functions;

    protected:
        Module(Engine* engine);

    public:
        virtual ~Module();

        v8::Local<v8::ObjectTemplate> GenerateObject();

        virtual std::string GetPackageName() = 0;
    };

} // namespace scripter