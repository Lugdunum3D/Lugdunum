#pragma once

#include <memory>
#include <cstdarg>
#include <string>
#include <unordered_map>
#include <vector>

#include <lug/System/Export.hpp>
#include <lug/System/SourceInfo.hpp>
#include <lug/System/Utils.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/DefaultFormatter.hpp>
#include <lug/System/Logger/FileHandler.hpp>
#include <lug/System/Logger/StdoutHandler.hpp>

#include <lug/System/Debug.hpp>


namespace lug {
namespace System {

class Logger;

using HandlerPtr = std::shared_ptr<priv::Handler>;
using LoggerPtr = std::shared_ptr<Logger>;
using HandlerInitList = std::initializer_list <HandlerPtr>;
using FormatterPtr = std::shared_ptr<Formatter>;

class LUG_SYSTEM_API Logger {
public:
    Logger(const std::string& loggerName, HandlerPtr single_sink);
    Logger(const std::string& name, HandlerInitList initList);

    // create logger with given name, handlers and the default pattern formatter
    // all other ctors will call this one
    template<class It>
    inline Logger(const std::string &loggerName, const It &begin, const It &end):
            _name(loggerName),
            _handlers(begin, end),
            _formatter(std::make_shared<DefaultFormatter>()) {

        /*_errHandler = [this](const std::string &msg) {
            this->_defaultErrHandler(msg);
        };*/
    }

    virtual ~Logger(void) {}

    void defaultErrHandler(const std::string& msg) {
        // TODO: Handle error
        (void)msg;
    }

    template<typename... Args>
    inline void log(Level::enumLevel lvl, const char *fmt, const Args &... args) {
    /*    if (!shouldLog(lvl))
            return;*/

        try {
            priv::Message logMsg(_name, lvl);
            logMsg.raw.write(fmt, args...);
            handle(logMsg);
        }
        catch (const Exception& e) {
            defaultErrHandler(e.what());
        }
        catch (...) {
            defaultErrHandler("Unknown exception");
        }
    }

    template<typename... Args>
    inline void log(Level::enumLevel lvl, const char *msg) {
        /*if (!shouldLog(lvl))
            return;*/
        try {
            priv::Message logMsg(_name, lvl);
            logMsg.raw << msg;
            handle(logMsg);
        }
        catch (const std::exception &ex) {
            defaultErrHandler(ex.what());
        }
        catch (...) {
            defaultErrHandler("Unknown exception");
        }

    }

    template<typename T>
    inline void log(Level::enumLevel lvl, const T &msg) {
        /*if (!shouldLog(lvl))
            return;*/
        try {
            priv::Message logMsg(_name, lvl);
            logMsg.raw << msg;
            handle(logMsg);
        }
        catch (const std::exception &ex) {
            defaultErrHandler(ex.what());
        }
        catch (...) {
            defaultErrHandler("Unknown exception");
        }
    }

    template<typename... Args>
    inline void debug(const char *fmt, const Args &... args) {
        log(Level::Debug, fmt, args...);
    }

    template<typename... Args>
    inline void info(const char *fmt, const Args &... args) {
        log(Level::Info, fmt, args...);
    }


    template<typename... Args>
    inline void warn(const char *fmt, const Args &... args) {
        log(Level::Warning, fmt, args...);
    }

    template<typename... Args>
    inline void error(const char *fmt, const Args &... args) {
        log(Level::Error, fmt, args...);
    }

    template<typename... Args>
    inline void fatal(const char *fmt, const Args &... args) {
        log(Level::Fatal, fmt, args...);
    }


    template<typename T>
    inline void debug(const T &msg) {
        log(Level::Debug, msg);
    }

    template<typename T>
    inline void info(const T &msg) {
        log(Level::Info, msg);
    }

    template<typename T>
    inline void warn(const T &msg) {
        log(Level::Warning, msg);
    }

    template<typename T>
    inline void error(const T &msg) {
        log(Level::Error, msg);
    }

    template<typename T>
    inline void fatal(const T &msg) {
        log(Level::Fatal, msg);
    }

    /*static std::string getChannelName(Channel::enumChannel c);
    static std::string getLevelName(Level::enumLevel t);
*/
    bool shouldLog(Level::enumLevel) const;

    void setLevel(Level::enumLevel);
    Level::enumLevel getLevel() const;
    const std::string &getName() const;
    void setPattern(const std::string &);
    void setFormatter(FormatterPtr formatterPtr);

    // error handler
    // void set_error_handler(log_err_handler);
    // log_err_handler error_handler();

    // automatically call flush() if message level >= log_level
    //void flushOn(Level::enumLevel log_level);

    void handle(priv::Message& msg);

    void flush();

    const std::vector<HandlerPtr> &handlers() const;

private:
    virtual void logMessage(priv::Message &message);
    //virtual void _set_pattern(const std::string &);
    //virtual void _set_formatter(formatter_ptr);

    // default error handler: print the error to stderr with the max rate of 1 message/minute
    //virtual void _default_err_handler(const std::string &msg);

    // return true if the given message level should trigger a flush
    //bool _should_flush_on(const priv::Message &);

    const std::string _name;
    std::vector<HandlerPtr> _handlers;
    FormatterPtr _formatter;
    //priv::LevelAtomic _flushLevel;
    priv::LevelAtomic _level;
    //log_err_handler _err_handler;
    //std::atomic<time_t> _lastErrTime;
};


class LUG_SYSTEM_API LoggerFacility {
public:
    static void registerLogger(const std::string& loggerName, LoggerPtr logger) {
        _loggers[loggerName] = logger;
    }
    static LoggerPtr getLogger(const std::string& loggerName) {
        return _loggers[loggerName];
    }

private:
    static std::unordered_map<std::string, LoggerPtr> _loggers;
};


template<typename T, typename... Args>
inline LoggerPtr makeLogger(const std::string &loggerName, Args... args) {
    static_assert(std::is_base_of<priv::Handler, T>::value, "T must derive from Handler");
    HandlerPtr handler = std::make_shared<T>(args...);
    LoggerPtr logger = std::make_shared<Logger>(loggerName, handler);
    LoggerFacility::registerLogger(loggerName, logger);
    return logger;
}

inline LoggerPtr makeBasicLogger(const std::string &loggerName, const filename_t &filename, bool truncate) {
    return makeLogger<FileHandler>(loggerName, filename, truncate);
}

inline LoggerPtr makeStdoutLogger(const std::string &loggerName) {
    return makeLogger<StdoutHandler>(loggerName);
}

/*
#define LUG_LOG(logger, channel, type, verbosity, ...)\
do {\
    logger->log(channel, type, verbosity, { __FILE__, LUG_SYSTEM_FUNCTION_NAME, __LINE__}, __VA_ARGS__); \
} while (0)

// Define each type as its own macro to facilitate calling
#define LUG_LOG_INFO(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Level::Info, verbosity, __VA_ARGS__)

#define LUG_LOG_WARNING(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Level::Warning, verbosity, __VA_ARGS__)

#define LUG_LOG_ERROR(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Level::Error, verbosity, __VA_ARGS__)

#define LUG_LOG_FATAL(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Level::Fatal, verbosity, __VA_ARGS__)

#define LUG_LOG_ASSERT(logger, channel, verbosity, ...)\
    LUG_LOG(logger, channel, lug::System::Level::Assert, verbosity, __VA_ARGS__)

} // namespace System

#if !defined(LUG_NO_SHORT_LOG)

    lug::System::Logger &getLogger();

    #define LOGINF(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Level::Info, 1, __VA_ARGS__); \
    } while (0)

    #define LOGWRN(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Level::Warning, 1, __VA_ARGS__); \
    } while (0)

    #define LOGERR(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Level::Error, 1, __VA_ARGS__); \
    } while (0)

    #define LOGFAT(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Level::Fatal, 1, __VA_ARGS__); \
    } while (0)

    #define LOGASR(...)\
    do {\
        lug::System::Logger *logger = &lug::getLogger(); \
        LUG_LOG(logger, lug::System::Logger::Channel::User, lug::System::Level::Assert, 1, __VA_ARGS__); \
    } while (0)

#endif
*/
} // namespace lug
} // namespace system

#include <lug/System/Logger.inl>
