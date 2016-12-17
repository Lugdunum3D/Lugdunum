#include <lug/System/Logger.hpp>
#include <algorithm>
#include <ctime>

namespace lug {
namespace System {

Logger Logger::logger("internal");

Logger::Logger(const std::string& loggerName) : _name(loggerName) {}

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
