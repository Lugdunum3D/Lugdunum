#pragma once

#include <iostream>
#include <lug/System/Logger/Handler.hpp>
#include <gmock/gmock.h>

namespace lug {
namespace System {

class MockHandler : public Handler {
public:
    MockHandler(const std::string& name) : Handler(name) {
    }
    virtual ~MockHandler() {
    }

    MOCK_METHOD1(handle, void(const priv::Message& msg));
    MOCK_METHOD0(flush, void());
};

} // namespace lug
} // namespace System
