#pragma once

#include <cstddef>
#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

class LUG_SYSTEM_API Stack {
public:
    struct Mark {
        void* current;
        lug::System::Memory::Area::Page* currentPage;
    };

public:
    Stack(lug::System::Memory::Area::IArea* area);
    Stack(const Stack&) = delete;
    Stack(Stack&&) = default;

    Stack& operator=(const Stack&) = delete;
    Stack& operator=(Stack&&) = default;

    ~Stack() = default;

    void* allocate(size_t size, size_t alignment, size_t offset);
    void free(void* ptr);
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

} // Allocator
} // Memory
} // System
} // lug
