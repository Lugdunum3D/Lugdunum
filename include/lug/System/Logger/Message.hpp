#pragma once

#include <lug/System/Logger/Common.hpp>

namespace lug {
namespace System {
namespace priv {

struct Message {
    Message() = default;
    Message(const std::string& _loggerName, Level::enumLevel _level): loggerName(_loggerName), level(_level) {

    }

    const std::string loggerName;
    Level::enumLevel level;
    fmt::MemoryWriter raw;
    fmt::MemoryWriter formatted;
};

}
}
}
