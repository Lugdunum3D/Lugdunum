#include <iostream>
#include <lug/System/Logger/OstreamHandler.hpp>
#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {
namespace Logger {

OstreamHandler::OstreamHandler(const std::string& name, std::ostream& out) : Handler(name), _os(out) {}

void OstreamHandler::handle(const priv::Message& msg) {
    _os << msg.formatted.c_str();
}

void OstreamHandler::flush() {
    _os << std::flush;
}

StdoutHandler::StdoutHandler(const std::string& name) : OstreamHandler(name, std::cout) {}
StderrHandler::StderrHandler(const std::string& name) : OstreamHandler(name, std::cerr) {}

} // Logger
} // System
} // lug
