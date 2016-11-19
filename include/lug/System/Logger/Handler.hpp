#pragma once

#include <lug/System/Logger/Message.hpp>

namespace lug {
namespace System {
namespace priv {

class Handler {
public:
    Handler() = default;
    virtual ~Handler() = default;

    void log(const Message& msg)
    {
        handle(msg);
    }
    
    virtual void flush() = 0;

protected:
    virtual void handle(const Message& msg) = 0;
};

} // namespace priv
} // namespace lug
} // namespace System
