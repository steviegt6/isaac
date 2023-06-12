// This file is part of Tomat.Testament.Lua, a fork of Lua and part of the
// Testament Project hosted within the mono-repository located at
// <https://github.com/steviegt6/isaac>.
// 
// Tomat.Testament.Lua is released under the MIT License. As such, it is subject
// to the terms written in the `LICENSE-MIT` file located within the project
// root.

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

        util::log::info("Tomat.Testament.Lua (Lua5.3.3r.dll) by Tomat, version %s\n", TOMAT_TESTAMENT_LUA_VERSION);
        util::log::info("This is a fork of Lua and thus is licensed under the MIT License.\n");
        util::log::info("See <https://github.com/steviegt6/isaac> and <https://www.lua.org/> for more information.\n");
        util::log::info("\n");
        util::log::info("Tomat.Testament.API is free software licensed under the GNU Lesser General Public License, version 3.\n");
        util::log::info("Tomat.Testament is free software licensed under the GNU General Public License, version 3.\n");
        util::log::info("See <https://github.com/steviegt6/isaac> for more information.\n");
        util::log::info("\n");
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
