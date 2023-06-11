#include <string>
#include <windows.h>
#include "dllmain.h"

// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        initialize_console("Tomat.Testament");

        printf("Hello, world!\n");
        printf("h_module: %p\n", h_module);
        printf("ul_reason_for_call: %lu\n", ul_reason_for_call);
        printf("lp_reserved: %p\n", lp_reserved);
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
