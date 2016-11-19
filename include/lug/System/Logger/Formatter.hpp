#pragma once

#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {

class Formatter {
public:
    Formatter() = default;
    virtual ~Formatter () {};

    virtual void format(priv::Message &msg) = 0;
};

} // namespace lug
} // namespace System
