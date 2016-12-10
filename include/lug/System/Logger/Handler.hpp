#pragma once

#include <memory>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/Logger/Formatter.hpp>
#include <lug/System/Logger/LoggingFacility.hpp>

namespace lug {
namespace System {

class Handler {
public:
    Handler(const std::string& name) : _name(name), _formatter(std::make_unique<Formatter>("%v\n")) {}
    virtual ~Handler() {};

    void setFormatter(std::unique_ptr<Formatter> formatter) {
        _formatter = std::move(formatter);
    }

    void format(priv::Message& msg) {
      _formatter->format(msg);
    }

    virtual void flush() = 0;
    virtual void handle(const priv::Message& msg) = 0;

protected:
    std::string _name;
    std::unique_ptr<Formatter> _formatter;
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
