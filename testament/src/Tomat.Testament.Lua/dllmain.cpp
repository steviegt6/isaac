#include "dllmain.h"

#include <string>
#include <windows.h>

#include "util/constants/constants.h"
#include "util/log/log.h"

// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(HMODULE, const DWORD ul_reason_for_call, LPVOID)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        initialize_console("Tomat.Testament");

        /*util::log::debug("debug\n");
        util::log::info("info\n");
        util::log::warn("warn\n");
        util::log::error("error\n");*/

        util::log::info("Tomat.Testament.Lua (Lua5.3.3r.dll) by Tomat, version %s\n", TOMAT_TESTAMENT_LUA_VERSION);
        util::log::info("This is a fork of Lua and is thus licensed under the MIT License.\n");
        util::log::info("See <https://github.com/steviegt6/isaac> and <https://www.lua.org/> for more information.\n");
        util::log::info("\n");
        util::log::info("Tomat.Testament.API is free software licensed under the GNU Lesser General Public License, version 3.\n");
        util::log::info("Tomat.Testament is free software licensed under the GNU General Public License, version 3.\n");
        util::log::info("See <https://github.com/steviegt6/isaac> for more information.\n");
        util::log::info("\n");

        /*printf("Hello, world!\n");
        printf("h_module: %p\n", h_module);
        printf("ul_reason_for_call: %lu\n", ul_reason_for_call);
        printf("lp_reserved: %p\n", lp_reserved);*/
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
