#include <lug/System/Memory/FreeList.hpp>
#include <memory>
#include <lug/System/Debug.hpp>

namespace lug {
namespace System {
namespace Memory {

FreeList::FreeList(size_t size) : _size(size) {
    LUG_ASSERT(_size > sizeof(FreeList::Element*), "The size is not enough for this implementation of freelist");
}

bool FreeList::grow(void* start, void* end, size_t alignment, size_t offset) {
    // First, align the start pointer
    {
        start = static_cast<char*>(start) + offset;

        LUG_ASSERT(start <= end, "Start was greater than end");

        size_t size = static_cast<size_t>(static_cast<char*>(end) - static_cast<char*>(start)) + 1;

        if (!std::align(alignment, _size - offset, start, size)) {
            return false;
        }

        start = static_cast<char*>(start) - offset;
    }

    // Second create the linked list
    {
        Element* it = static_cast<Element*>(start);
        _nextFree = it;

        LUG_ASSERT(start <= end, "Start was greater than end");

        size_t size = static_cast<size_t>(static_cast<char*>(end) - static_cast<char*>(start)) + 1;

        for (size_t i = 1, count = size / _size; i < count; ++i) {
            void* const next = static_cast<char*>(static_cast<void*>(it)) + _size;

            it->next = static_cast<Element*>(next);
            it = static_cast<Element*>(next);
        }

        it->next = nullptr;
    }

    return true;
}

void* FreeList::allocate() {
    if (!_nextFree) {
        return nullptr;
    }

    Element* const head = _nextFree;
    _nextFree = _nextFree->next;
    return head;
}

void FreeList::free(void* ptr) {
    Element* const head = static_cast<Element*>(ptr);

    head->next = _nextFree;
    _nextFree = head;
}

void FreeList::reset() {
    _nextFree = nullptr;
}

} // Memory
} // System
} // lug
