#pragma once

#include <iostream>
#include <lug/System/Logger/Handler.hpp>

namespace lug {
namespace System {

class OstreamHandler : public Handler {
public:
    OstreamHandler(const std::string& name, std::ostream& out) : Handler(name), _os(out) {
    }
    virtual ~OstreamHandler() {
    }

protected:
    virtual void handle(const priv::Message& msg) {
        _os << msg.formatted.c_str();
    }
    virtual void flush() {
        _os << std::flush;
    }

    std::ostream& _os;
};

class StdoutHandler : public OstreamHandler {
public:
    StdoutHandler(const std::string& name) : OstreamHandler(name, std::cout) {}
};

class StderrHandler : public OstreamHandler {
public:
    StderrHandler(const std::string& name) : OstreamHandler(name, std::cerr) {}
};

} // namespace lug
} // namespace System
