#pragma once

#include <memory>
#include <cstdarg>
#include <string>
#include <atomic>
#include <unordered_map>
#include <lug/System/Export.hpp>
#include <lug/System/SourceInfo.hpp>
#include <lug/System/Utils.hpp>

#define FMT_HEADER_ONLY
#if defined(LUG_FMT_EXTERNAL)
    #include <fmt/format.h>
    #include <fmt/ostream.h>
#else
    #include <lug/Ext/fmt/format.h>
    #include <lug/Ext/fmt/ostream.h>
#endif

namespace lug {
namespace System {

// Generate channels list
// enum: RENDER_API, RENDER_ENGINE, PHYSICS_ENGINE, etc...
// map pair: { RENDER_API, "RENDER_API" }, { RENDER_ENGINE, "RENDER_ENGINE" }, etc...
#define LUG_LOG_CHANNELS(PROCESS)   \
    PROCESS(Vulkan),                \
    PROCESS(Render),                \
    PROCESS(Physics),               \
    PROCESS(Script),                \
    PROCESS(Network),               \
    PROCESS(Sound),                 \
    PROCESS(Memory),                \
    PROCESS(Window),                \
    PROCESS(Math),                  \
    PROCESS(User),                  \

#define LUG_LOG_LEVELS(PROCESS)     \
    PROCESS(Off),                   \
    PROCESS(Debug),                 \
    PROCESS(Info),                  \
    PROCESS(Warning),               \
    PROCESS(Error),                 \
    PROCESS(Fatal),                 \
    PROCESS(Assert),                \

#define LUG_LOG_ENUM(CHANNEL) CHANNEL
#define LUG_LOG_MAP_PAIR_C(CHANNEL) { Logger::Channel::CHANNEL, #CHANNEL }
#define LUG_LOG_MAP_PAIR_T(CHANNEL) { Level::CHANNEL, #CHANNEL }

#if defined(LUG_SYSTEM_WINDOWS)
using filename_t = std::wstring;
#else
using filename_t = std::string;
#endif

namespace Channel {
typedef enum {
    LUG_LOG_CHANNELS(LUG_LOG_ENUM)
} enumChannel;
};

namespace Level {
typedef enum {
    LUG_LOG_LEVELS(LUG_LOG_ENUM)
} enumLevel;
};

} // namespace lug
} // namespace system
