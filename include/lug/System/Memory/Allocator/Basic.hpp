#pragma once

#include <cstddef>
#include <lug/System/Export.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

class LUG_SYSTEM_API Basic {
public:
    Basic() = default;
    Basic(const Basic&) = delete;
    Basic(Basic&&) = default;

    Basic& operator=(const Basic&) = delete;
    Basic& operator=(Basic&&) = default;

    ~Basic() = default;

    void* allocate(size_t size, size_t alignment, size_t offset) const;
    void free(void* ptr) const;
    void reset() const;

    size_t getSize(void *ptr) const;
};

}
}
}
}
