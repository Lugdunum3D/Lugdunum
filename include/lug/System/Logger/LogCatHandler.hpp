#pragma once

#include <android/log.h>
#include <lug/System/Logger/Handler.hpp>

namespace lug {
namespace System {
namespace Logger {

class LUG_SYSTEM_API LogCatHandler : public Handler {
public:
    LogCatHandler(const std::string& name);

    LogCatHandler(const LogCatHandler&) = delete;
    LogCatHandler(LogCatHandler&&) = delete;

    LogCatHandler& operator=(const LogCatHandler&) = delete;
    LogCatHandler& operator=(LogCatHandler&&) = delete;

    ~LogCatHandler() = default;

private:
    void handle(const priv::Message& msg);
    void flush();

    android_LogPriority lugLevelToLogCatPrio(Level level);
};

} // Logger
} // System
} // lug
