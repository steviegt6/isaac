#include "clr.h"

#include <cassert>
#include <cstdio>
#include <iostream>

#include "dllmain.h"
#include "host.h"

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

    printf("Starting CLR... (C++)\n");

    host_start_clr();
}
