#include <lug/System/Logger/LogCatHandler.hpp>

namespace lug {
namespace System {
namespace Logger {

LogCatHandler::LogCatHandler(const std::string& name) : Handler(name) {}

void LogCatHandler::handle(const priv::Message& msg) {
    __android_log_write(lugLevelToLogCatPrio(msg.level), msg.loggerName.c_str(), msg.formatted.c_str());
}

void LogCatHandler::flush() {
    // Not applicable
}

android_LogPriority LogCatHandler::lugLevelToLogCatPrio(Level level) {
    switch (level) {
        case lug::System::Logger::Level::Off:       return ANDROID_LOG_SILENT;
        case lug::System::Logger::Level::Debug:     return ANDROID_LOG_DEBUG;
        case lug::System::Logger::Level::Info:      return ANDROID_LOG_INFO;
        case lug::System::Logger::Level::Warning:   return ANDROID_LOG_WARN;
        case lug::System::Logger::Level::Error:     return ANDROID_LOG_ERROR;
        case lug::System::Logger::Level::Fatal:     return ANDROID_LOG_FATAL;
        case lug::System::Logger::Level::Assert:    return ANDROID_LOG_FATAL;
    }
    return ANDROID_LOG_UNKNOWN;
}

} // Logger
} // System
} // lug
