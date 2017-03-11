#include <lug/System/Logger/LoggingFacility.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/System/Logger/Handler.hpp>

namespace lug {
namespace System {
namespace Logger {

static std::unordered_map<std::string, std::unique_ptr<Logger>> loggers{};
static std::unordered_map<std::string, std::unique_ptr<Handler>> handlers{};

void LoggingFacility::registerLogger(const std::string& loggerName, std::unique_ptr<Logger> logger) {
    loggers[loggerName] = std::move(logger);
}

Logger* LoggingFacility::getLogger(const std::string& loggerName) {
    return loggers[loggerName].get();
}

void LoggingFacility::registerHandler(const std::string& handlerName, std::unique_ptr<Handler> handler) {
    handlers[handlerName] = std::move(handler);
}

Handler* LoggingFacility::getHandler(const std::string& handlerName) {
    return handlers[handlerName].get();
}

void LoggingFacility::clear() {
    handlers.clear();
    loggers.clear();
}

} // Logger
} // System
} // lug
