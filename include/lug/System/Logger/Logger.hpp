#pragma once

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include <lug/System/Export.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/LoggingFacility.hpp>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {
namespace Logger {

class Handler;

class LUG_SYSTEM_API Logger {
public:
    explicit Logger(const std::string& loggerName);

    Logger(const Logger&) = delete;
    Logger(Logger&&) = default;

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = default;

    ~Logger() = default;

    void addHandler(Handler* handler);
    void addHandler(const std::string& name);

    void defaultErrHandler(const std::string& msg);
    void defaultErrHandler(const std::exception& ex);

    template<typename T>
    void log(std::string source, Level lvl, const T& msg);

    template<typename... Args, typename T>
    void log(std::string source, Level lvl, const T& fmt, Args&&... args);

    template<typename T, typename... Args>
    void debug(std::string source, const T& fmt, Args&&... args);

    template<typename T, typename... Args>
    void info(std::string source, const T& fmt, Args&&... args);

    template<typename T, typename... Args>
    void warn(std::string source, const T& fmt, Args&&... args);

    template<typename T, typename... Args>
    void error(std::string source, const T& fmt, Args&&... args);

    template<typename T, typename... Args>
    void fatal(std::string source, const T& fmt, Args&&... args);

    template<typename T, typename... Args>
    void assrt(std::string source, const T& fmt, Args&&... args);

    void muteLevel(std::string source, Level level);
    void unmuteLevel(std::string source, Level level);
    bool getLoggingAllowed(std::string source, Level level);

    const std::string& getName() const;
    void handle(priv::Message& msg);
    void flush();

    static Logger& getInternalLogger();

protected:
    const std::string _name;
    std::set<Handler*> _handlers;

private:
    std::unordered_map<std::string, std::vector<Level>> _srcLevels;
};

#include <lug/System/Logger/Logger.inl>

#define LUG_LOG ::lug::System::Logger::Logger::getInternalLogger()

} // Logger
} // System
} // lug
