#include <lug/System/Logger/Logger.hpp>
#include <lug/System/Logger/Handler.hpp>

namespace lug {
namespace System {
namespace Logger {

#define LUG_LOG_ENUM(CHANNEL) case Level::CHANNEL: return os << #CHANNEL;

std::ostream& operator<<(std::ostream& os, Level level) {
    switch (level) {
        LUG_LOG_LEVELS(LUG_LOG_ENUM)
    }
    return os << "Unknown";
}

#undef LUG_LOG_ENUM

Logger::Logger(const std::string& loggerName) : _name(loggerName) {}

void Logger::addHandler(Handler* handler) {
    _handlers.insert(handler);
}

void Logger::addHandler(const std::string& name) {
    _handlers.insert(LoggingFacility::getHandler(name));
}

void Logger::defaultErrHandler(const std::string& msg) {
    log(Level::Fatal, "Exception in logger {}: {}", _name, msg);
}

void Logger::defaultErrHandler(const std::exception& ex) {
    defaultErrHandler(ex.what());
}

const std::string& Logger::getName() const {
    return _name;
}

void Logger::handle(priv::Message& msg) {
    for (auto& handler : _handlers) {
        if (handler->shouldLog(msg.level)) {
            handler->format(msg);
            handler->handle(msg);
        }
    }
}

void Logger::flush() {
    for (auto& handler : _handlers) {
        handler->flush();
    }
}

Logger& Logger::getInternalLogger() {
    static Logger logger("internal");

    return logger;
}

} // namespace Logger
} // namespace System
} // namespace lug
