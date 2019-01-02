/*
 * MIT License
 *
 * Copyright (c) 2019 Patrik M. Rosenström
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
#include "scripter/utils/Path.h"

#include "scripter/Logger.h"

#include <stdlib.h>
#include <string.h>

#include <linux/limits.h>

namespace scripter {

    static char buffer[PATH_MAX] = {};

    String Path::GetFileName(const String& path)
    {
        String result = path.substr(path.find_last_of('/') + 1);

        return result;
    }

    String Path::GetFileExtension(const String& path)
    {
        String result = path.substr(path.find_last_of('.') + 1);

        return result;
    }

    String Path::GetDirectoryPath(const String& path)
    {
        String result = path.substr(0, path.find_last_of('/'));

        return result;
    }

    String Path::GetFullPath(const String& path)
    {
        memset(buffer, 0, PATH_MAX);

        realpath(path.c_str(), buffer);
        String result(buffer);

        return result;
    }

    String Path::Append(const String& path, const String& path2)
    {
        String result = path;
        if (path[path.length() - 1] != '/')
            result.append(1, '/');
        result.append(path2);

        return result;
    }

} // namespace scripter