#include <lug/System/Logger.hpp>
#include <algorithm>
#include <ctime>

namespace lug {
namespace System {

std::unordered_map<std::string, std::unique_ptr<Logger>> LoggerFacility::_loggers{};
std::unordered_map<std::string, std::unique_ptr<Handler>> LoggerFacility::_handlers{};

Logger::Logger(const std::string &loggerName) : _name(loggerName) {}

void Logger::setPattern(const std::string &pattern) {
    for (auto& handler : _handlers) {
        handler->setFormatter(std::make_unique<Formatter>(pattern));
    }
}

const std::string& Logger::getName() const {
    return _name;
}

void Logger::setLevel(Level::enumLevel level) {
    _level.store(level);
}


Level::enumLevel Logger::getLevel() const {
    return static_cast<Level::enumLevel>(_level.load(std::memory_order_relaxed));
}

bool Logger::shouldLog(Level::enumLevel msg_level) const {
    return msg_level >= _level.load(std::memory_order_relaxed);
}

void Logger::handle(priv::Message& msg) {
    for (auto& handler : _handlers) {
        handler->format(msg);
        handler->handle(msg);
    }
}

void Logger::flush() {
    for (auto& handler : _handlers) {
        handler->flush();
    }
}

} // namespace System
} // namespace lug
