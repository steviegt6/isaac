#pragma once

struct lua_hooks
{
    void* api;
    extern "C" int add(lua_hooks* self, int a, int b);
};

extern "C" void* host_start_clr(char* module_path);
