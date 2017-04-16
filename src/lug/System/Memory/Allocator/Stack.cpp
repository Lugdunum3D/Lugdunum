#include <lug/System/Memory/Allocator/Stack.hpp>
#include <cstdlib>
#include <memory>
#include <lug/System/Debug.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

Stack::Stack(lug::System::Memory::Area::IArea* area) : _area{area}, _currentPage{_area->requestNextPage()}, _firstPage{_currentPage} {
    if (_currentPage) {
        _current = _currentPage->start;
    }
}

void* Stack::allocate(size_t size, size_t alignment, size_t offset) {
    LUG_ASSERT(size > offset, "The size must be greater than the offset");

    // Adapt the size to store the size and the current pointer at the beginning of the block
    const size_t newSize = size + sizeof(size_t) + sizeof(void*);
    const size_t newOffset = offset + sizeof(size_t) + sizeof(void*);

    void* const oldCurrent = _current;

    while (_current) {
        // Try to allocate memory on the current page
        _current = static_cast<char*>(_current) + newOffset;

        if (_current <= _currentPage->end) {
            size_t sizeLeft = static_cast<char*>(_currentPage->end) - static_cast<char*>(_current) + 1;

            if (std::align(alignment, newSize - newOffset, _current, sizeLeft)) {
                _current = static_cast<char*>(_current) + newSize - newOffset;

                // Store the size and the current pointer
                *static_cast<size_t*>(static_cast<void*>(static_cast<char*>(_current) - newSize + sizeof(void*))) = size;
                *reinterpret_cast<void**>(static_cast<char*>(_current) - newSize) = oldCurrent;

                return static_cast<char*>(_current) - size;
            }
        }

        // Out of memory on this page, just request a new one and try to reallocate
        _currentPage = _currentPage->next = _currentPage->next ? _currentPage->next : _area->requestNextPage();
        _current = _currentPage ? _currentPage->start : nullptr;
    }

    return nullptr;
}

void Stack::free(void* ptr) {
    LUG_ASSERT(static_cast<char*>(ptr) + getSize(ptr) == _current, "Deallocation with stack allocator not in LIFO order");

    void* const oldCurrent = reinterpret_cast<void**>(static_cast<char*>(ptr) - sizeof(size_t))[-1];

    // We need to find the correct page from which this pointer come from
    while (_currentPage && (_currentPage->start > oldCurrent || _currentPage->end < oldCurrent)) {
        _currentPage = _currentPage->prev;
    }

    LUG_ASSERT(_currentPage, "Deallocation of pointer from the wrong arena");

    _current = oldCurrent;
}

void Stack::reset() {
    _currentPage = _firstPage;

    if (_currentPage) {
        _current = _currentPage->start;
    }
}

Stack::Mark Stack::getMark() const {
    return {_current, _currentPage};
}

void Stack::rewind(const Stack::Mark& mark) {
    _current = mark.current;
    _currentPage = mark.currentPage;
}

size_t Stack::getSize(void* ptr) const {
    return static_cast<size_t*>(ptr)[-1];
}

} // Allocator
} // Memory
} // System
} // lug
