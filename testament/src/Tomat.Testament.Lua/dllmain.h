#pragma once

#include <windows.h>

// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved);

void initialize_console(LPCSTR title);
