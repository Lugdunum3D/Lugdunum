#pragma once

#include <memory>
#include <lug/System/Logger/Message.hpp>
#include <lug/System/Logger/Formatter.hpp>

namespace lug {
namespace System {

class Handler {
public:
    Handler(const std::string& name) : _name(name), _formatter(std::make_unique<Formatter>("%v\n")) {}
    virtual ~Handler() = default;

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

} // namespace lug
} // namespace System
