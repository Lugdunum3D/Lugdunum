#include <lug/System/Memory/Allocator/Linear.hpp>
#include <cstdlib>
#include <memory>
#include <lug/System/Debug.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

Linear::Linear(lug::System::Memory::Area::IArea* area) : _area{area}, _currentPage{_area->requestNextPage()}, _firstPage{_currentPage} {
    if (_currentPage) {
        _current = _currentPage->start;
    }
}

void* Linear::allocate(size_t size, size_t alignment, size_t offset) {
    LUG_ASSERT(size > offset, "The size must be greater than the offset");

    // Adapt the size to store the size at the beginning of the block
    const size_t newSize = size + sizeof(size_t);
    const size_t newOffset = offset + sizeof(size_t);

    while (_current) {
        // Try to allocate memory on the current page
        _current = static_cast<char*>(_current) + newOffset;
        if (_current <= _currentPage->end) {
            size_t sizeLeft = static_cast<char*>(_currentPage->end) - static_cast<char*>(_current) + 1;
            if (std::align(alignment, newSize - newOffset, _current, sizeLeft)) {
                _current = static_cast<char*>(_current) + newSize - newOffset;

                // Store the size
                *static_cast<size_t*>(static_cast<void*>(static_cast<char*>(_current) - newSize)) = size;

                return static_cast<char*>(_current) - size;
            }
        }

        // Out of memory on this page, just request a new one and try to reallocate
        _currentPage = _currentPage->next = _currentPage->next ? _currentPage->next : _area->requestNextPage();
        _current = _currentPage ? _currentPage->start : nullptr;
    }

    return nullptr;
}

void Linear::free(void*) const {
    // Do nothing here
}

void Linear::reset() {
    _currentPage = _firstPage;

    if (_currentPage) {
        _current = _currentPage->start;
    }
}

Linear::Mark Linear::getMark() const {
    return {_current, _currentPage};
}

void Linear::rewind(const Linear::Mark& mark) {
    _current = mark.current;
    _currentPage = mark.currentPage;
}

size_t Linear::getSize(void* ptr) const {
    return static_cast<size_t*>(ptr)[-1];
}

} // Allocator
} // Memory
} // System
} // lug
