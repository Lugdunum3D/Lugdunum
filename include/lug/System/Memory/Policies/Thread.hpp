#pragma once

#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Policies {

class SingleThreadPolicy {
public:
    void enter() const;
    void leave() const;
};

template <class SynchronizationPrimitive>
class MultiThreadPolicy {
public:
    void enter();
    void leave();

private:
    SynchronizationPrimitive _primitive;
};

#include <lug/System/Memory/Policies/Thread.inl>

} // Policies
} // Memory
} // System
} // lug
