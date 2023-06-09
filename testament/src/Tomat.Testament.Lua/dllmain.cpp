#include <string>
#include <windows.h>
#include "dllmain.h"

// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        /*std::string message = "Hello, world!\n";
message += "h_module: " + std::to_string(reinterpret_cast<std::uintptr_t>(h_module)) + "\n";
message += "ul_reason_for_call: " + std::to_string(ul_reason_for_call) + "\n";
message += "lp_reserved: " + std::to_string(reinterpret_cast<std::uintptr_t>(lp_reserved)) + "\n";

MessageBoxA(nullptr, message.c_str(), "Tomat.Testament.Lua", MB_OK);*/

        initialize_console("Tomat.Testament");

        printf("Hello, world!\n");
        printf("h_module: %p\n", h_module);
        printf("ul_reason_for_call: %lu\n", ul_reason_for_call);
        printf("lp_reserved: %p\n", lp_reserved);
        break;

    default: break;
    }

    return TRUE;
}

void initialize_console(const LPCSTR title)
{
    AllocConsole();
    SetConsoleTitleA(title);

    FILE* dev_null;
    (void)freopen_s(&dev_null, "CONIN$", "r", stdin);
    (void)freopen_s(&dev_null, "CONOUT$", "w", stderr);
    (void)freopen_s(&dev_null, "CONOUT$", "w", stdout);

    const HANDLE console_handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD lp_mode;
    GetConsoleMode(console_handle, &lp_mode);
    SetConsoleMode(console_handle, ENABLE_EXTENDED_FLAGS | (lp_mode & ~ENABLE_QUICK_EDIT_MODE));
}
