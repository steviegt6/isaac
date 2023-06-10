#include "clr.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <minwindef.h>

#include "dllmain.h"
#include <hostfxr.h>
#include <coreclr_delegates.h>

hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;

bool clr_initialized = false;

bool get_clr_initialized()
{
    return clr_initialized;
}

void start_clr()
{
    assert(!get_clr_initialized());
    clr_initialized = true;

    const std::string module_path = get_module_path();
    printf("Using module path: %s\n", module_path.c_str());

    // get directory of this module
    const size_t last_slash_idx = module_path.rfind('\\');
    assert(last_slash_idx != std::string::npos);
    const std::string module_dir = module_path.substr(0, last_slash_idx);
    printf("Using module directory: %s\n", module_dir.c_str());

    /*if (!load_hostfxr())
    {
        assert(false && "Failure: load_hostfxr()");
        return;
    }*/
}

std::string get_module_path()
{
    char path[MAX_PATH];
    return std::string(path, GetModuleFileNameA(nullptr, path, MAX_PATH));
}

void* load_library(const char_t* path)
{
    const HMODULE h = LoadLibraryW(path);
    assert(h != nullptr);
    return h;
}

void* get_export(void* h, const char* name)
{
    void* f = GetProcAddress(static_cast<HMODULE>(h), name);
    assert(f != nullptr);
    return f;
}

// Using the nethost library, discover the location of hostfxr and get exports
bool load_hostfxr()
{
    // Pre-allocate a large buffer for the path to hostfxr
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    const int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
    if (rc != 0)
        return false;

    // Load hostfxr and get desired exports
    void* lib = load_library(buffer);
    init_fptr = static_cast<hostfxr_initialize_for_runtime_config_fn>(get_export(lib, "hostfxr_initialize_for_runtime_config"));
    get_delegate_fptr = static_cast<hostfxr_get_runtime_delegate_fn>(get_export(lib, "hostfxr_get_runtime_delegate"));
    close_fptr = static_cast<hostfxr_close_fn>(get_export(lib, "hostfxr_close"));

    return (init_fptr && get_delegate_fptr && close_fptr);
}

load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t* config_path)
{
    // Load .NET Core
    void* load_assembly_and_get_function_pointer = nullptr;
    hostfxr_handle cxt = nullptr;
    int rc = init_fptr(config_path, nullptr, &cxt);
    if (rc != 0 || cxt == nullptr)
    {
        std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
        close_fptr(cxt);
        return nullptr;
    }

    // Get the load assembly function pointer
    rc = get_delegate_fptr(
        cxt,
        hdt_load_assembly_and_get_function_pointer,
        &load_assembly_and_get_function_pointer);
    if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
        std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;

    close_fptr(cxt);
    return static_cast<load_assembly_and_get_function_pointer_fn>(load_assembly_and_get_function_pointer);
}
