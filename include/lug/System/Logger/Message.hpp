#pragma once

#include <lug/System/Logger/Common.hpp>

namespace lug {
namespace System {
namespace Logger {

namespace priv {

class Message {
public:
    Message() = default;
    Message(const std::string& _loggerName, Level _level): loggerName(_loggerName), level(_level) {}

    Message(const Message&) = default;
    Message(Message&&) = default;

    Message& operator=(const Message&) = default;
    Message& operator=(Message&&) = default;

    ~Message() = default;

    const std::string loggerName;
    Level level;

    fmt::MemoryWriter raw{};
    fmt::MemoryWriter formatted{};
};

} // priv

} // Logger
} // System
} // lug
