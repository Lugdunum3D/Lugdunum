#pragma once

#include <lug/System/Export.hpp>

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#define FMT_HEADER_ONLY
#if defined(False)
    #undef False

    #include <fmt/format.h>
    #include <fmt/ostream.h>

    #define False 0
#else
    #include <fmt/format.h>
    #include <fmt/ostream.h>
#endif

#define LUG_LOG_LEVELS(PROCESS)     \
    PROCESS(Debug, 0x01)            \
    PROCESS(Info, 0x02)             \
    PROCESS(Warning, 0x04)          \
    PROCESS(Error, 0x08)            \
    PROCESS(Fatal, 0x10)            \
    PROCESS(Assert, 0x20)           

namespace lug {
namespace System {
namespace Logger {

#define LUG_LOG_ENUM(CHANNEL, VALUE) CHANNEL = VALUE,
enum class Level : uint8_t {
    LUG_LOG_LEVELS(LUG_LOG_ENUM)
};
#undef LUG_LOG_ENUM

LUG_SYSTEM_API std::ostream& operator<<(std::ostream& os, Level level);

} // Logger
} // System
} // lug
