#pragma once

#include <cstdarg>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <lug/System/Debug.hpp>
#include <lug/System/Export.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/FileHandler.hpp>
#include <lug/System/Logger/FileHandler.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/Logger/StdoutHandler.hpp>
#include <lug/System/SourceInfo.hpp>
#include <lug/System/Utils.hpp>


namespace lug {
namespace System {

class Logger;

class LUG_SYSTEM_API LoggerFacility {
public:
    static inline void registerLogger(const char* loggerName, std::unique_ptr<Logger> logger) {
        _loggers[loggerName] = std::move(logger);
    }
    static inline Logger* getLogger(const char* loggerName) {
        return _loggers[loggerName].get();
    }

    static inline void registerHandler(const char* handlerName, std::unique_ptr<Handler> handler) {
        _handlers[handlerName] = std::move(handler);
    }
    static inline Handler* getHandler(const char* handlerName) {
        return _handlers[handlerName].get();
    }

private:
    static std::unordered_map<const char*, std::unique_ptr<Logger>> _loggers;
    static std::unordered_map<const char*, std::unique_ptr<Handler>> _handlers;
};


class LUG_SYSTEM_API Logger {
public:
    Logger(const std::string& loggerName);

    virtual ~Logger(void) {}

    void addHandler(Handler* handler) {
        _handlers.push_back(handler);
    }

    void addHandler(const char* name) {
        _handlers.push_back(LoggerFacility::getHandler(name));
    }

    template<typename T, typename... Args>
    void addHandler(Args... args) {
        Handler* handler = std::make_shared<T>(args...);
        _handlers.push_back(handler);
    }

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

    // automatically call flush() if message level >= log_level
    //void flushOn(Level::enumLevel log_level);

    void handle(priv::Message& msg);

    void flush();

    // const std::vector<HandlerPtr> &handlers() const;

private:
    virtual void logMessage(priv::Message &message);
    //virtual void _set_pattern(const std::string &);
    //virtual void _set_formatter(formatter_ptr);

    // default error handler: print the error to stderr with the max rate of 1 message/minute
    //virtual void _default_err_handler(const std::string &msg);

    // return true if the given message level should trigger a flush
    //bool _should_flush_on(const priv::Message &);

    const std::string _name;
    std::vector<Handler*> _handlers;
    //priv::LevelAtomic _flushLevel;
    std::atomic_int _level;
    //log_err_handler _err_handler;
    //std::atomic<time_t> _lastErrTime;
};


inline Logger* makeLogger(const char* loggerName) {
    std::unique_ptr<Logger> logger = std::make_unique<Logger>(loggerName);
    Logger* loggerRawPtr = logger.get();
    LoggerFacility::registerLogger(loggerName, std::move(logger));
    return loggerRawPtr;
}

template<typename T, typename... Args>
inline T* makeHandler(const char* handlerName, Args... args) {
    std::unique_ptr<T> handler = std::make_unique<T>(handlerName, args...);
    T* handlerRawPtr = handler.get();
    LoggerFacility::registerHandler(handlerName, std::move(handler));
    return handlerRawPtr;
}

} // namespace lug
} // namespace system

#include <lug/System/Logger.inl>
