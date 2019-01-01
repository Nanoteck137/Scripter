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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define SCRIPTER_ASSERT(x, ...)                                                \
    if (!(x))                                                                  \
    {                                                                          \
        SCRIPTER_LOG_CRITICAL("ASSERTION FAILED:");                            \
        SCRIPTER_LOG_CRITICAL("File: {0}:{1} in function '{2}'", __FILE__,     \
                              __LINE__, __FUNCTION__);                         \
        SCRIPTER_LOG_CRITICAL("Condition: {0}", #x);                           \
        SCRIPTER_LOG_CRITICAL("Message: " __VA_ARGS__);                        \
        SCRIPTER_TRAP_DEBUGGER;                                                \
    }

#define SCRIPTER_LOG_INFO(...)                                                 \
    ::scripter::Logger::GetScripterLogger()->info(__VA_ARGS__)

#define SCRIPTER_LOG_WARNING(...)                                              \
    ::scripter::Logger::GetScripterLogger()->warn(__VA_ARGS__)

#define SCRIPTER_LOG_ERROR(...)                                                \
    ::scripter::Logger::GetScripterLogger()->error(__VA_ARGS__)

#define SCRIPTER_LOG_CRITICAL(...)                                             \
    ::scripter::Logger::GetScripterLogger()->critical(__VA_ARGS__)

#define JS_LOG_INFO(...) ::scripter::Logger::GetJSLogger()->info(__VA_ARGS__)

#define JS_LOG_WARNING(...) ::scripter::Logger::GetJSLogger()->warn(__VA_ARGS__)

#define JS_LOG_ERROR(...) ::scripter::Logger::GetJSLogger()->error(__VA_ARGS__)

#define JS_LOG_CRITICAL(...)                                                   \
    ::scripter::Logger::GetJSLogger()->critical(__VA_ARGS__)

#define MODULE_LOG_INFO(...)                                                   \
    ::scripter::Logger::GetModuleLogger()->info(__VA_ARGS__)

#define MODULE_LOG_WARNING(...)                                                \
    ::scripter::Logger::GetModuleLogger()->warn(__VA_ARGS__)

#define MODULE_LOG_ERROR(...)                                                  \
    ::scripter::Logger::GetModuleLogger()->error(__VA_ARGS__)

#define MODULE_LOG_CRITICAL(...)                                               \
    ::scripter::Logger::GetModuleLogger()->critical(__VA_ARGS__)

namespace scripter {

    /**
     * Logger
     *
     * A singleton pattern class to handle all the spdlogs
     */
    class Logger
    {
    private:
        static std::shared_ptr<spdlog::logger> s_ScripterLogger;
        static std::shared_ptr<spdlog::logger> s_JSLogger;
        static std::shared_ptr<spdlog::logger> s_ModuleLogger;

    private:
        Logger();

    public:
        ~Logger();

        /**
         * Initializes the singleton
         */
        static void Initialize();

        /**
         * Deinitializes the singleton
         */
        static void Deinitialize();

        /**
         * Returns the scripter logger
         */
        static std::shared_ptr<spdlog::logger>& GetScripterLogger()
        {
            return s_ScripterLogger;
        }

        /**
         * Returns the javascript logger
         */
        static std::shared_ptr<spdlog::logger>& GetJSLogger()
        {
            return s_JSLogger;
        }

        /**
         * Returns the module logger
         */
        static std::shared_ptr<spdlog::logger>& GetModuleLogger()
        {
            return s_ModuleLogger;
        }
    };

} // namespace scripter