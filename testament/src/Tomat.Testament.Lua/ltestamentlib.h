#pragma once

#include "lua/lua.h"

#define LUA_TESTAMENTLIBNAME	"testament"

LUAMOD_API int luaopen_testament(lua_State* state);
