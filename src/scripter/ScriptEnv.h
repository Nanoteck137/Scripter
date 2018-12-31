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
#include "scripter/Module.h"

#include <v8.h>

namespace scripter {

    /**
     * ScriptEnv
     *
     * This is a wrapper around V8's context, a script enviroment where you can
     * run scripts
     *
     */
    class ScriptEnv
    {
    private:
        Engine* m_Engine;
        v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>>
            m_Context;

    public:
        /**
         * Constructor
         * @param engine the engine this script environment should use
         */
        ScriptEnv(Engine* engine);

        ~ScriptEnv();

        /**
         * Makes this script environment active
         */
        void Enable();

        /**
         * Deactives this script environment
         */
        void Disable();

        /**
         * Imports an module
         * @param module the module to import
         */
        void ImportModule(Module* module);

        /**
         * Compiles and runs a javascript script
         */
        v8::MaybeLocal<v8::Value> CompileAndRun(const std::string& code);

        /**
         * Returns the V8 Context.
         */
        v8::Local<v8::Context> GetContext();

        /**
         * Returns a handle to an global function in the enviroment.
         * @param name the name of the function
         */
        v8::MaybeLocal<v8::Function> GetFunction(const std::string& name);
    };

} // namespace scripter