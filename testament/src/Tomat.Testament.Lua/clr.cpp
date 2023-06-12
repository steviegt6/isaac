#include <stdlib.h>
#include <cstdio>

#include "clr.h"

#include <cassert>
#include <iostream>

#include "dllmain.h"
#include <string>

// #define NETHOST_USE_AS_STATIC
#include "coreclr_delegates.h"
#include "hostfxr.h"
#include "nethost.h"

using string_t = std::basic_string<char_t>;

bool clr_initialized = false;

bool get_clr_initialized()
{
    return clr_initialized;
}

hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;

bool load_hostfxr();
load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t* config_path);

char_t* string_to_char_t(std::string str)
{
    const auto chars = new char_t[str.length() + 1];
    std::copy(str.begin(), str.end(), chars);
    return chars;
}

void start_clr()
{
    if (get_clr_initialized())
    {
        printf("Attempted to initialize CLR despite it already being initialized!\n");
        return;
    }

    clr_initialized = true;

    char_t module_path[MAX_PATH];
    assert(GetModuleFileNameW(nullptr, module_path, MAX_PATH) != 0);

    string_t module_directory(module_path);
    module_directory = module_directory.substr(0, module_directory.find_last_of(L"\\/"));

    string_t config_path(module_directory);
    config_path.append(L"\\Tomat.Testament.runtimeconfig.json");

    string_t assembly_path(module_directory);
    assembly_path.append(L"\\Tomat.Testament.dll");

    const auto type_name = L"Tomat.Testament.Native, Tomat.Testament";
    const auto method_name = L"Initialize";

    printf("Module directory: %ls\n", module_directory.c_str());
    printf("Config path: %ls\n", config_path.c_str());
    printf("Assembly path: %ls\n", assembly_path.c_str());
    printf("Type name: %ls\n", type_name);
    printf("Method name: %ls\n", method_name);
    printf("Starting CLR... (C++)\n");
    printf("Load hostfxr: %d\n", load_hostfxr());

    const load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path.c_str());
    assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");

    // printf("Load assembly and get function pointer: %p\n", load_assembly_and_get_function_pointer);

    //component_entry_point_fn initialize = nullptr;
    struct init_args
    {
    };
    typedef void (CORECLR_DELEGATE_CALLTYPE *custom_entry_point_fn)(init_args args);
    custom_entry_point_fn initialize = nullptr;
    const int rc = load_assembly_and_get_function_pointer(
        reinterpret_cast<const char_t*>(assembly_path.c_str()),
        reinterpret_cast<const char_t*>(type_name),
        reinterpret_cast<const char_t*>(method_name),
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        reinterpret_cast<void**>(&initialize)
    );

    if (rc != 0 || initialize == nullptr)
    {
        printf("Failed to load assembly and get function pointer! (C++)\n");
        std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
    }

    // printf("Initialize: %p\n", initialize);

    init_args args = {};
    initialize(args);

    printf("CLR started, initialize called! (C++)\n");
}

void* load_library(const char_t* path)
{
    const HMODULE h = LoadLibraryW(path);
    assert(h != nullptr);
    return h;
}

void* get_export(void* h, const char* name)
{
    void* f = GetProcAddress(static_cast<HMODULE>(h), name); // NOLINT(clang-diagnostic-microsoft-cast)
    assert(f != nullptr);
    return f;
}

bool load_hostfxr()
{
    // Pre-allocate a large buffer for the path to hostfxr
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    const int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
    if (rc != 0)
        return false;

    printf("Found hostfxr at: %ls\n", buffer);

    // Load hostfxr and get desired exports
    void* lib = load_library(buffer);
    init_fptr = static_cast<hostfxr_initialize_for_runtime_config_fn>(get_export(lib, "hostfxr_initialize_for_runtime_config")); // NOLINT(clang-diagnostic-microsoft-cast)
    get_delegate_fptr = static_cast<hostfxr_get_runtime_delegate_fn>(get_export(lib, "hostfxr_get_runtime_delegate")); // NOLINT(clang-diagnostic-microsoft-cast)
    close_fptr = static_cast<hostfxr_close_fn>(get_export(lib, "hostfxr_close")); // NOLINT(clang-diagnostic-microsoft-cast)

    // printf("hostfxr_initialize_for_runtime_config: %p\n", init_fptr);
    // printf("hostfxr_get_runtime_delegate: %p\n", get_delegate_fptr);
    // printf("hostfxr_close: %p\n", close_fptr);

    return lib && init_fptr && get_delegate_fptr && close_fptr;
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
    return static_cast<load_assembly_and_get_function_pointer_fn>(load_assembly_and_get_function_pointer); // NOLINT(clang-diagnostic-microsoft-cast)
}
