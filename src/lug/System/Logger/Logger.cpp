#include <lug/System/Logger.hpp>
#include <algorithm>
#include <ctime>

namespace lug {
namespace System {

Logger::Logger(const std::string& loggerName) : _name(loggerName) {}

/**
 * Sets a given pattern on all the handlers of the Logger.
 * Warning: this will effectively modify the handlers, and it will change
 * handlers that might be used somewhere else, like in an other Logger instance.
 * @param pattern The new pattern
 */
void Logger::setPattern(const std::string& pattern) {
    for (auto& handler : _handlers) {
        handler->setFormatter(std::make_unique<Formatter>(pattern));
    }
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
