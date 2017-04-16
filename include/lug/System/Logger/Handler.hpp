#pragma once

#include <atomic>
#include <memory>
#include <lug/System/Export.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/LoggingFacility.hpp>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {
namespace Logger {

class LUG_SYSTEM_API Handler {
public:
    explicit Handler(const std::string& name);

    Handler(const Handler&) = delete;
    Handler(Handler&&) = delete;

    Handler& operator=(const Handler&) = delete;
    Handler& operator=(Handler&&) = delete;

    virtual ~Handler() = default;

    void setFormatter(std::unique_ptr<Formatter> formatter);
    void setPattern(const std::string& pattern);
    void format(priv::Message& msg);

    virtual void flush() = 0;
    virtual void handle(const priv::Message& msg) = 0;

    bool shouldLog(Level level) const;
    void setLevel(Level level);
    Level getLevel() const;

protected:
    std::string _name;
    std::unique_ptr<Formatter> _formatter;
    Level _level;
};

template<typename T, typename... Args>
T* makeHandler(const std::string& handlerName, Args&&... args);

#include <lug/System/Logger/Handler.inl>

} // Logger
} // System
} // lug

