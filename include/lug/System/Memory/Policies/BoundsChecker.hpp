#pragma once

#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Policies {

class LUG_SYSTEM_API NoBoundsChecking {
public:
    static const size_t SizeFront = 0;
    static const size_t SizeBack = 0;

    void guardFront(void* ptr, size_t size) const;
    void guardBack(void* ptr, size_t size) const;

    void checkFront(void* ptr, size_t size) const;
    void checkBack(void* ptr, size_t size) const;
    void checkReset() const;
};

#include <lug/System/Memory/Policies/BoundsChecker.inl>

}
}
}
}
