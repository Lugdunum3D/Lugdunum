#pragma once

#include <memory>
#include <string>
#include <set>
#include <string>

#include <lug/System/Debug.hpp>
#include <lug/System/Export.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/LoggingFacility.hpp>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/SourceInfo.hpp>
#include <lug/System/Utils.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API Logger {
public:
    Logger(const std::string& loggerName);
    virtual ~Logger() {}

    virtual void addHandler(Handler* handler) {
        _handlers.insert(handler);
    }

    virtual void addHandler(const std::string& name) {
        _handlers.insert(LoggingFacility::getHandler(name));
    }

    virtual void defaultErrHandler(const std::string& msg) {
        // TODO: Handle error
        (void)msg;
    }

    template<typename T>
    inline void log(Level::enumLevel lvl, const T& msg) {
        try {
            priv::Message logMsg(_name, lvl);
            logMsg.raw.write("{}", msg);
            handle(logMsg);
        }
        catch (const std::exception& ex) {
            defaultErrHandler(ex.what());
        }
        catch (...) {
            defaultErrHandler("Unknown exception");
        }
    }

    template<typename... Args, typename T>
    inline void log(Level::enumLevel lvl, const T& fmt, const Args &... args) {
        try {
            priv::Message logMsg(_name, lvl);
            logMsg.raw.write(fmt, args...);
            handle(logMsg);
        }
        catch (const std::exception& ex) {
            defaultErrHandler(ex.what());
        }
        catch (...) {
            defaultErrHandler("Unknown exception");
        }
    }

    template<typename T, typename... Args>
    inline void debug(const T& fmt, const Args&... args) {
        log(Level::Debug, fmt, args...);
    }

    template<typename T, typename... Args>
    inline void info(const T& fmt, const Args&... args) {
        log(Level::Info, fmt, args...);
    }

    template<typename T, typename... Args>
    inline void warn(const T& fmt, const Args&... args) {
        log(Level::Warning, fmt, args...);
    }

    template<typename T, typename... Args>
    inline void error(const T& fmt, const Args&... args) {
        log(Level::Error, fmt, args...);
    }

    template<typename T, typename... Args>
    inline void fatal(const T& fmt, const Args&... args) {
        log(Level::Fatal, fmt, args...);
    }

    template<typename T, typename... Args>
    inline void assrt(const T& fmt, const Args&... args) {
        log(Level::Assert, fmt, args...);
    }

    virtual bool shouldLog(Level::enumLevel) const;

    virtual void setLevel(Level::enumLevel);
    virtual Level::enumLevel getLevel() const;
    virtual const std::string& getName() const;
    virtual void setPattern(const std::string&);

    virtual void handle(priv::Message& msg);

    virtual void flush();

protected:
    const std::string _name;
    std::set<Handler*> _handlers;
    std::atomic_int _level;
};


inline Logger* makeLogger(const std::string& loggerName) {
    std::unique_ptr<Logger> logger = std::make_unique<Logger>(loggerName);
    Logger* loggerRawPtr = logger.get();
    LoggingFacility::registerLogger(loggerName, std::move(logger));
    return loggerRawPtr;
}

} // namespace lug
} // namespace system

#include <lug/System/Logger.inl>
