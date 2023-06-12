// This file is part of Tomat.Testament.Lua, a fork of Lua and part of the
// Testament Project hosted within the mono-repository located at
// <https://github.com/steviegt6/isaac>.
// 
// Tomat.Testament.Lua is released under the MIT License. As such, it is subject
// to the terms written in the `LICENSE-MIT` file located within the project
//root.

#include "util.h"

#include <cassert>
#include <iostream>

namespace util
{
    char* char_t_to_char(const char_t* str)
    {
        const size_t len = wcslen(str) + 1;
        const auto buffer = new char[len];
        size_t converted_chars = 0;
        const unsigned int length = wcstombs_s(&converted_chars, buffer, len, str, _TRUNCATE);
        assert((length == 0 || length != len) && "Failure: wcstombs_s()");
        return buffer;
    }
}
