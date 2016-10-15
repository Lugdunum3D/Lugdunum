#pragma once

#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Policies {

class LUG_SYSTEM_API SingleThreadPolicy {
public:
    void enter() const;
    void leave() const;
};

template<class SynchronizationPrimitive>
class LUG_SYSTEM_API MultiThreadPolicy {
public:
    void enter();
    void leave();

private:
    SynchronizationPrimitive _primitive;
};

#include <lug/System/Memory/Policies/Thread.inl>

}
}
}
}
