#include <cstdio>

#include "clr.h"

#include "dllmain.h"
#include "host.h"
#include <PathCch.h>

bool clr_initialized = false;

bool get_clr_initialized()
{
    return clr_initialized;
}

void start_clr()
{
    if (get_clr_initialized())
    {
        printf("Attempted to initialize CLR despite it already being initialized!\n");
        return;
    }

    clr_initialized = true;

    char module_path[MAX_PATH];
    GetModuleFileNameA(nullptr, module_path, MAX_PATH);
    printf("Module path: %s\n", module_path);
    
    printf("Starting CLR... (C++)\n");
    host_start_clr(module_path);
    printf("CLR started! (C++)\n");
}
