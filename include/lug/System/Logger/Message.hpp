#pragma once

#include <lug/System/Logger/Common.hpp>

namespace lug {
namespace System {
namespace priv {

class Message {
public:
    Message() = default;
    Message(const std::string& _loggerName, Level _level): loggerName(_loggerName), level(_level) {}

    const std::string loggerName;
    Level level;
    fmt::MemoryWriter raw;
    fmt::MemoryWriter formatted;
};

}
}
}
