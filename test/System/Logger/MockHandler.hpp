#pragma once

#include <lug/System/Logger/Handler.hpp>
#include <gmock/gmock.h>

namespace lug {
namespace System {
namespace Logger {

class MockHandler : public Handler {
public:
    MockHandler(const std::string& name) : Handler(name) {}
    ~MockHandler() {}

    MOCK_METHOD1(handle, void(const priv::Message& msg));
    MOCK_METHOD0(flush, void());
};

} // Logger
} // System
} // lug
