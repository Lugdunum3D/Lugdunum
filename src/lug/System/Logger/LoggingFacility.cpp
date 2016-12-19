#include <lug/System/Logger/LoggingFacility.hpp>
#include <lug/System/Logger.hpp>
#include <lug/System/Logger/Handler.hpp>

namespace lug {
namespace System {

std::unordered_map<std::string, std::unique_ptr<Logger>> LoggingFacility::_loggers{};
std::unordered_map<std::string, std::unique_ptr<Handler>> LoggingFacility::_handlers{};

void LoggingFacility::registerLogger(const std::string& loggerName, std::unique_ptr<Logger> logger) {
    _loggers[loggerName] = std::move(logger);
}

Logger* LoggingFacility::getLogger(const std::string& loggerName) {
    return _loggers[loggerName].get();
}

void LoggingFacility::registerHandler(const std::string& handlerName, std::unique_ptr<Handler> handler) {
    _handlers[handlerName] = std::move(handler);
}

Handler* LoggingFacility::getHandler(const std::string& handlerName) {
    return _handlers[handlerName].get();
}

void LoggingFacility::clear() {
    _handlers.clear();
    _loggers.clear();
}

}
}
