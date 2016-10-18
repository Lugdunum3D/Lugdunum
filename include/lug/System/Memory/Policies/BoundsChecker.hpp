#pragma once

#include <cstring>
#include <lug/System/Export.hpp>
#include <lug/System/Debug.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Policies {

class NoBoundsChecking {
public:
    static constexpr size_t SizeFront = 0;
    static constexpr size_t SizeBack = 0;

    void guardFront(void* ptr, size_t size) const;
    void guardBack(void* ptr, size_t size) const;

    void checkFront(void* ptr, size_t size) const;
    void checkBack(void* ptr, size_t size) const;
};

class SimpleBoundsChecking {
public:
    static constexpr size_t SizeFront = 4;
    static constexpr size_t SizeBack = 4;

    void guardFront(void* ptr, size_t size) const;
    void guardBack(void* ptr, size_t size) const;

    void checkFront(void* ptr, size_t size) const;
    void checkBack(void* ptr, size_t size) const;

private:
    static constexpr char* MagicFront = "\xDE\xAD";
    static constexpr char* MagicBack = "\xBE\xEF";
};

#include <lug/System/Memory/Policies/BoundsChecker.inl>

}
}
}
}
