// This file is part of Tomat.Testament.Lua, a fork of Lua and part of the
// Testament Project hosted within the mono-repository located at
// <https://github.com/steviegt6/isaac>.
// 
// Tomat.Testament.Lua is released under the MIT License. As such, it is subject
// to the terms written in the `LICENSE-MIT` file located within the project
// root.

#define LUA_LIB

#include "ltestamentlib.h"

#include <windows.h>

#include "lua/lauxlib.h"
#include "lua/lua.h"

// As of now, this file goes more or less unused. We do the bare minimum of
// creating a new library with the name "testament", allowing for users to check
// if a `testament` object exists within the context of their Lua script.
// Additional commented out code demonstrates the basics for adding native
// functions and fields to the library, but is not used in the current
// implementation.

// This is a simple function that allows a user to create a message box from a
// lua script.
/*static int testament_message_box(lua_State* state)
{
    const int args = lua_gettop(state);
    if (args == 3)
    {
        const char* message = lua_tostring(state, 1);
        const char* title = lua_tostring(state, 2);
        const int type = lua_tointeger(state, 3);
        MessageBoxA(NULL, message, title, type);
    }
    else if (args == 2)
    {
        const char* message = lua_tostring(state, 1);
        const char* title = lua_tostring(state, 2);
        MessageBoxA(NULL, message, title, MB_OK);
    }
    else if (args == 1)
    {
        const char* message = lua_tostring(state, 1);
        MessageBoxA(NULL, message, "Testament", MB_OK);
    }
    else
    {
        MessageBoxA(NULL, "Testament", "Testament", MB_OK);
    }

    return 0;
}*/

// Here is where the script is added, with the key being the function name and
// the value being the function pointer.
// Note that the last entry in the array must be a NULL entry.
static const luaL_Reg testamentlib[] = {
    // {"messagebox", testament_message_box},
    {NULL, NULL}
};

// This is the function that should be called to actually create the library.
// In our case, we call this in `lua/lstate.c`'s `lua_newstate` function, all
// the way at the bottom. We do this because TBoI:R calls this function but not
// the regular function for loading every library, so we need a reliable place
// to call this function whenever TBoI:R creates a new Lua state.
LUAMOD_API int luaopen_testament(lua_State* state)
{
    luaL_newlib(state, testamentlib);
    /*lua_pushnumber(state, 1);
    lua_setfield(state, -2, "TESTAMENT");*/
    return 1;
}
