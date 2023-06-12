#include "log.h"

#include <cstdarg>
#include <stdio.h>
#include <windows.h>

namespace util
{
    namespace log
    {
        void set_console_color(const WORD color)
        {
            const HANDLE std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(std_handle, color);
        }

        void debug(const char* format, ...)
        {
            set_console_color(FOREGROUND_INTENSITY);
            va_list args;
            va_start(args, format);
            (void)vprintf_s(format, args);
            va_end(args);
        }

        void info(const char* format, ...)
        {
            set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            va_list args;
            va_start(args, format);
            (void)vprintf_s(format, args);
            va_end(args);
        }

        void warn(const char* format, ...)
        {
            set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            va_list args;
            va_start(args, format);
            (void)vprintf_s(format, args);
            va_end(args);
        }

        void error(const char* format, ...)
        {
            set_console_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
            va_list args;
            va_start(args, format);
            (void)fprintf_s(stderr, format, args);
            va_end(args);
        }
    }
}
