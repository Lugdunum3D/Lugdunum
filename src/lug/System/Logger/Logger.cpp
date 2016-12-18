#include <lug/System/Logger.hpp>

namespace lug {
namespace System {

Logger Logger::logger("internal");

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

} // namespace System
} // namespace lug
