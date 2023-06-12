#pragma once
#include "coreclr_delegates.h"
#include "hostfxr.h"

hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;
bool clr_initialized = false;

bool load_hostfxr();
load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t* config_path);
void* load_library(const char_t* path);
void* get_export(void* h, const char* name);

#ifdef __cplusplus
extern "C" {
#endif
bool get_clr_initialized();
void start_clr();
#ifdef __cplusplus
}
#endif
