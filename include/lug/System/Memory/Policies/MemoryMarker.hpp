#pragma once

#include <cstring>
#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Policies {

class NoMemoryMarking {
public:
    inline void markAllocation(void* ptr, size_t size) const;
    inline void markDeallocation(void* ptr, size_t size) const;
};

class SimpleMemoryMarking {
public:
    inline void markAllocation(void* ptr, size_t size) const;
    inline void markDeallocation(void* ptr, size_t size) const;

private:
    static constexpr uint8_t AllocationMagic = 0xCD;
    static constexpr uint8_t DeallocationMagic = 0xDD;
};

#include <lug/System/Memory/Policies/MemoryMarker.inl>

}
}
}
}
