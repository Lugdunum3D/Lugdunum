#pragma once

#define FMT_HEADER_ONLY
#if defined(LUG_FMT_EXTERNAL)
    #include <fmt/format.h>
    #include <fmt/ostream.h>
#else
    #include <lug/Ext/fmt/format.h>
    #include <lug/Ext/fmt/ostream.h>
#endif

#define LUG_LOG_LEVELS(PROCESS)     \
    PROCESS(Debug),                 \
    PROCESS(Info),                  \
    PROCESS(Warning),               \
    PROCESS(Error),                 \
    PROCESS(Fatal),                 \
    PROCESS(Assert),                \
    PROCESS(Off),                   \

#define LUG_LOG_ENUM(CHANNEL) CHANNEL

#if defined(LUG_SYSTEM_WINDOWS)
    using filename_t = std::wstring;
#else
    using filename_t = std::string;
#endif

namespace lug {
namespace System {

enum class Level {
    LUG_LOG_LEVELS(LUG_LOG_ENUM)
};

} // namespace System
} // namespace priv
