// This file is part of Tomat.Testament.Lua, a fork of Lua and part of the
// Testament Project hosted within the mono-repository located at
// <https://github.com/steviegt6/isaac>.
// 
// Tomat.Testament.Lua is released under the MIT License. As such, it is subject
// to the terms written in the `LICENSE-MIT` file located within the project
// root.

#pragma once

namespace util
{
    namespace log
    {
        void debug(const char* format, ...);
        void info(const char* format, ...);
        void warn(const char* format, ...);
        void error(const char* format, ...);
    }
}
