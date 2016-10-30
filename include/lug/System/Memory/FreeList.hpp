#pragma once

#include <cstdlib>
#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {

class LUG_SYSTEM_API FreeList {
public:
    explicit FreeList(size_t size);

    FreeList(const FreeList&) = default;
    FreeList(FreeList&&) = default;

    FreeList& operator=(const FreeList&) = default;
    FreeList& operator=(FreeList&&) = default;

    ~FreeList() = default;

    bool grow(void* start, void* end, size_t alignment, size_t offset);

    void* allocate();
    void free(void* ptr);
    void reset();

private:
    struct LUG_SYSTEM_API Element {
        Element* next;
    };

    size_t _size;
    Element* _nextFree{nullptr};
};

}
}
}
