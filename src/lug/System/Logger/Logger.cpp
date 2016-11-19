#include <algorithm>
#include <lug/System/Logger.hpp>
#include <ctime>

namespace lug {
namespace System {

//std::unordered_map<Channel::enumChannel, const char*> Logger::_channelNames = { LUG_LOG_CHANNELS(LUG_LOG_MAP_PAIR_C) };
//std::unordered_map<Level::enumLevel, const char*> Logger::_typeNames = { LUG_LOG_LEVELS(LUG_LOG_MAP_PAIR_T) };
std::unordered_map<std::string, LoggerPtr> LoggerFacility::_loggers{};

// ctor with handlers as init list
Logger::Logger(const std::string &loggerName, HandlerInitList initList) :
        Logger(loggerName, initList.begin(), initList.end()) {}


// ctor with single sink
Logger::Logger(const std::string &loggerName, HandlerPtr sink) :
        Logger(loggerName, {sink}) {}


inline void Logger::setFormatter(FormatterPtr formatter) {
    _formatter = formatter;
}

inline void Logger::setPattern(const std::string &pattern) {
    (void)pattern;
}




const std::string &Logger::getName() const {
    return _name;
}

void Logger::setLevel(Level::enumLevel level) {
    _level.store(level);
}

/*
inline void Logger::set_error_handler(spdlog::log_err_handler err_handler) {
    _errHandler = err_handler;
}

inline spdlog::log_err_handler Logger::error_handler() {
    return _errHandler;
}*/


/*inline void Logger::flushOn(level::level_enum log_level) {
    _flushLevel.store(log_level);
}
*/

Level::enumLevel Logger::getLevel() const {
    return static_cast<Level::enumLevel>(_level.load(std::memory_order_relaxed));
}

bool Logger::shouldLog(Level::enumLevel msg_level) const {
    return msg_level >= _level.load(std::memory_order_relaxed);
}

//
// protected virtual called at end of each user log call (if enabled) by the line_logger
//
void Logger::handle(priv::Message& msg) {
    _formatter->format(msg);
    for (auto& handler : _handlers) {
        //if (handler->shouldLog(msg.level)) {
            handler->log(msg);
        //}
    }

    //if (_should_flush_on(msg))
        //flush();
}
/*
inline void Logger::_set_pattern(const std::string &pattern) {
    _formatter = std::make_shared<pattern_formatter>(pattern);
}

inline void Logger::_set_formatter(formatter_ptr msg_formatter) {
    _formatter = msg_formatter;
}
*/
void Logger::flush() {
    for (auto& handler : _handlers) {
        handler->flush();
    }
}
// std::unordered_map<std::string, LoggerPtr> LoggerFacility::_loggers;

/*inline void Logger::_defaultErrHandler(const std::string &msg) {
    auto now = time(nullptr);
    if (now - _lastErrTime < 60)
        return;
    auto tm_time = details::os::localtime(now);
    char date_buf[100];
    std::strftime(date_buf, sizeof(date_buf), "%Y-%m-%d %H:%M:%S", &tm_time);
    priv::Message err_msg;
    //err_msg.formatted.write("[*** LOG ERROR ***] [{}] [{}] [{}]{}", name(), msg, date_buf, details::os::eol);
    //handers::stderr_sink_mt::instance()->log(err_msg);
    _lastErrTime = now;
}

inline bool Logger::_shouldFlushOn(const priv::Message &msg) {
    const auto flush_level = _flushLevel.load(std::memory_order_relaxed);
    return (msg.level >= flush_level) && (msg.level != level::off);
}*/

const std::vector<HandlerPtr> &Logger::handlers() const {
    return _handlers;
}

void Logger::logMessage(priv::Message &message) {
    (void)message;
}


} // namespace System
} // namespace lug
