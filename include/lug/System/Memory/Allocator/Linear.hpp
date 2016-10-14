#pragma once

#include <cstddef>
#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

class LUG_SYSTEM_API Linear {
public:
    Linear(lug::System::Memory::Area::IArea* area);
    Linear(const Linear&) = delete;
    Linear(Linear&&) = default;

    Linear& operator=(const Linear&) = delete;
    Linear& operator=(Linear&&) = default;

    ~Linear() = default;

    void* allocate(size_t size, size_t alignment, size_t offset);
    void free(void *ptr) const;
    void reset();

private:
    lug::System::Memory::Area::IArea* _area;

    void* _current = nullptr;
    lug::System::Memory::Area::Page* _currentPage;
    lug::System::Memory::Area::Page* _firstPage;
};

}
}
}
}
