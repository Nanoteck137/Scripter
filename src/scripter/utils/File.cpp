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
#include "scripter/utils/File.h"

#include "scripter/Logger.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace scripter {

    static char buffer[PATH_MAX] = {};

    String File::ReadFile(const String& filePath)
    {
        FILE* file = fopen(filePath.c_str(), "rt");
        SCRIPTER_ASSERT(file);

        if (!file)
        {
            SCRIPTER_LOG_ERROR("Could not open file '{0}'", filePath);
            return String();
        }

        fseek(file, 0, SEEK_END);
        int32 length = ftell(file);
        fseek(file, 0, SEEK_SET);

        String result(length, 0);

        fread(&result[0], 1, length, file);
        fclose(file);

        return result;
    }

    String File::GetFullPath(const String& path)
    {
        memset(buffer, 0, PATH_MAX);

        realpath(path.c_str(), buffer);
        String result(buffer);

        return result;
    }

} // namespace scripter