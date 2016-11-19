#pragma once

#include <iostream>
#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace System {

class StdoutHandler : public priv::Handler {
public:
    StdoutHandler() {
    }
    virtual ~StdoutHandler() {
    }

protected:
    virtual void handle(const priv::Message& msg) {
        std::cout << msg.formatted.c_str() << "\n";
    }
    virtual void flush() {
        std::cout << std::flush;
    }
};

} // namespace lug
} // namespace System
