#pragma once

#include <lug/System/Export.hpp>

#define FMT_HEADER_ONLY
#if defined(LUG_FMT_EXTERNAL)
    #include <fmt/format.h>
    #include <fmt/ostream.h>
#else
    #include <lug/Ext/fmt/format.h>
    #include <lug/Ext/fmt/ostream.h>
#endif

#define LUG_LOG_LEVELS(PROCESS)     \
    PROCESS(Debug)                  \
    PROCESS(Info)                   \
    PROCESS(Warning)                \
    PROCESS(Error)                  \
    PROCESS(Fatal)                  \
    PROCESS(Assert)                 \
    PROCESS(Off)                    \

namespace lug {
namespace System {

#define LUG_LOG_ENUM(CHANNEL) CHANNEL,
enum class Level : uint8_t {
    LUG_LOG_LEVELS(LUG_LOG_ENUM)
};
#undef LUG_LOG_ENUM

LUG_SYSTEM_API std::ostream& operator<<(std::ostream& os, Level level);

} // System
} // lug
