#pragma once

#include <ostream>
#include <lug/System/Export.hpp>
#include <lug/System/Logger/Handler.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API OstreamHandler : public Handler {
public:
    OstreamHandler(const std::string& name, std::ostream& out);

    OstreamHandler(const OstreamHandler&) = delete;
    OstreamHandler(OstreamHandler&&) = delete;

    OstreamHandler& operator=(const OstreamHandler&) = delete;
    OstreamHandler& operator=(OstreamHandler&&) = delete;

    virtual ~OstreamHandler() = default;

    void handle(const priv::Message& msg) final;
    void flush() final;

private:
    std::ostream& _os;
};

class LUG_SYSTEM_API StdoutHandler : public OstreamHandler {
public:
    StdoutHandler(const std::string& name);

    StdoutHandler(const StdoutHandler&) = delete;
    StdoutHandler(StdoutHandler&&) = delete;

    StdoutHandler& operator=(const StdoutHandler&) = delete;
    StdoutHandler& operator=(StdoutHandler&&) = delete;

    ~StdoutHandler() = default;
};

class LUG_SYSTEM_API StderrHandler : public OstreamHandler {
public:
    StderrHandler(const std::string& name);

    StderrHandler(const StderrHandler&) = delete;
    StderrHandler(StderrHandler&&) = delete;

    StderrHandler& operator=(const StderrHandler&) = delete;
    StderrHandler& operator=(StderrHandler&&) = delete;

    ~StderrHandler() = default;
};

} // namespace lug
} // namespace System
