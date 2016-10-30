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
    struct Mark {
        void* current;
        lug::System::Memory::Area::Page* currentPage;
    };

public:
    Linear(lug::System::Memory::Area::IArea* area);
    Linear(const Linear&) = delete;
    Linear(Linear&&) = default;

    Linear& operator=(const Linear&) = delete;
    Linear& operator=(Linear&&) = default;

    ~Linear() = default;

    void* allocate(size_t size, size_t alignment, size_t offset);
    void free(void* ptr) const;
    void reset();

    // Dangerous operations
    // Don't free previously allocated pointer after this mark !
    Mark getMark() const;
    void rewind(const Mark& mark);

    size_t getSize(void* ptr) const;

private:
    lug::System::Memory::Area::IArea* const _area;

    void* _current{nullptr};
    lug::System::Memory::Area::Page* _currentPage{nullptr};
    lug::System::Memory::Area::Page* _firstPage{nullptr};
};

}
}
}
}
