#pragma once

namespace util
{
    namespace log
    {
        void debug(const char* format, ...);
        void info(const char* format, ...);
        void warn(const char* format, ...);
        void error(const char* format, ...);
    }
}
