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
#include "scripter/Module.h"

#include <string>

namespace scripter {

    /**
     * NativeModuleImporter
     *
     * This is a importer for native modules and handles all the "low-level"
     * details.
     *
     * This is a singleton pattern so theres always just one instance of this
     * class and its initialized when the V8 is initalized.
     */
    class NativeModuleImporter
    {
    public:
        friend class Engine;

    private:
        static NativeModuleImporter* s_Instance;
        typedef Module* (*CreateModuleFunc)(Engine*);

        std::unordered_map<String, void*> m_Handles;
        std::vector<String> m_SearchPaths;

    private:
        NativeModuleImporter();

    public:
        ~NativeModuleImporter();

        /**
         * Finds and imports a native module a ".so" or a ".dll" file.
         * @param engine
         * @param moduleName the name of the module to find
         */
        Module* ImportModule(Engine* engine, const String& moduleName);

    public:
        /**
         * Returns the instance of this class
         */
        static NativeModuleImporter* Get();

    private:
        static void Initialize();
        static void Deinitialize();
    };

} // namespace scripter