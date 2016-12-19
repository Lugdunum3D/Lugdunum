#pragma once

#include <atomic>
#include <memory>
#include <lug/System/Export.hpp>
#include <lug/System/Logger/Common.hpp>
#include <lug/System/Logger/LoggingFacility.hpp>

namespace lug {
namespace System {

namespace priv {
class Message;
}
class Formatter;

class LUG_SYSTEM_API Handler {
public:
    Handler(const std::string& name);
    virtual ~Handler();

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
    std::atomic_int _level;
};

#include <lug/System/Logger/Handler.inl>

} // namespace lug
} // namespace System

