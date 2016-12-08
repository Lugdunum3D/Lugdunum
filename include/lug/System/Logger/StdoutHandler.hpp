#pragma once

#include <iostream>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace System {

class StdoutHandler : public Handler {
public:
    StdoutHandler(const std::string& name) : Handler(name) {
    }
    virtual ~StdoutHandler() {
    }

protected:
    virtual void handle(const priv::Message& msg) {
        std::cout << msg.formatted.c_str();
    }
    virtual void flush() {
        std::cout << std::flush;
    }
};

} // namespace lug
} // namespace System
