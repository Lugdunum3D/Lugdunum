#pragma once

#include <atomic>
#include <memory>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/LoggingFacility.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API Handler {
public:
    Handler(const std::string& name);
    virtual ~Handler();

    void setFormatter(std::unique_ptr<Formatter> formatter);
    void setPattern(const std::string& pattern);
    void format(priv::Message& msg);

    virtual void flush() = 0;
    virtual void handle(const priv::Message& msg) = 0;

    bool shouldLog(Level::enumLevel level) const;
    void setLevel(Level::enumLevel level);
    Level::enumLevel getLevel() const;

protected:
    std::string _name;
    std::unique_ptr<Formatter> _formatter;
    std::atomic_int _level;
};

template<typename T, typename... Args>
inline T* makeHandler(const std::string& handlerName, Args&... args) {
    std::unique_ptr<T> handler = std::make_unique<T>(handlerName, args...);
    T* handlerRawPtr = handler.get();
    LoggingFacility::registerHandler(handlerName, std::move(handler));
    return handlerRawPtr;
}

} // namespace lug
} // namespace System
