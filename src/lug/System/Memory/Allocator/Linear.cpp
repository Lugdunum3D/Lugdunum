#include <lug/System/Memory/Allocator/Linear.hpp>
#include <cstdlib>
#include <memory>
#include <lug/System/Debug.hpp>

lug::System::Memory::Allocator::Linear::Linear(lug::System::Memory::Area::IArea* area) : _area{area}, _currentPage{_area->requestNextPage()}, _firstPage{_currentPage} {
    if (_currentPage) {
        _current = _currentPage->start;
    }
}

void* lug::System::Memory::Allocator::Linear::allocate(size_t size, size_t alignment, size_t offset) {
    LUG_ASSERT(size > offset, "The size must be greater than the offset");

    if (!_current) {
        return nullptr;
    }

    // Try to allocate memory on the current page
    _current = static_cast<char*>(_current) + offset;
    if (_current <= _currentPage->end) {
        size_t sizeLeft = static_cast<char*>(_currentPage->end) - static_cast<char*>(_current) + 1;
        if (std::align(alignment, size - offset, _current, sizeLeft)) {
            _current = static_cast<char*>(_current) + size - offset;
            return static_cast<char*>(_current) - size;
        }
    }

    // Out of memory on this page, just request a new one and try to reallocate
    _currentPage = _area->requestNextPage();

    if (_currentPage) {
        _current = _currentPage->start;
        return allocate(size, alignment, offset);
    }

    _current = nullptr;
    return nullptr;
}

void lug::System::Memory::Allocator::Linear::free(void* ptr) const {
    // Do nothing here
}

void lug::System::Memory::Allocator::Linear::reset() {
    _currentPage = _firstPage;

    if (_currentPage) {
        _current = _currentPage->start;
    }
}
