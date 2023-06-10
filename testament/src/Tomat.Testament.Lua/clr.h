#pragma once

#include <string>

#define NETHOST_USE_AS_STATIC
#include <nethost.h>

#include <hostfxr.h>
#include <coreclr_delegates.h>

/**
 * \brief Whether the CLR has been initialized.
 * \remark Necessary because we use lua_newstate as a "hook", which may be
 *  called multiple times.
 */
bool get_clr_initialized();

void start_clr();
std::string get_module_path();
void* load_library(const char_t* path);
void* get_export(void* h, const char* name);
bool load_hostfxr();
load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t* config_path);
