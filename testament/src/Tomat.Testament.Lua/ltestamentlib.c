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

static int testament_message_box(lua_State* state)
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
}

static const luaL_Reg testamentlib[] = {
    {"messagebox", testament_message_box},
    {NULL, NULL}
};

LUAMOD_API int luaopen_testament(lua_State* state)
{
    luaL_newlib(state, testamentlib);
    /*lua_pushnumber(state, 1);
    lua_setfield(state, -2, "TESTAMENT");*/
    return 1;
}
