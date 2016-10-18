#include <lug/System/Memory/FreeList.hpp>
#include <memory>
#include <lug/System/Debug.hpp>

lug::System::Memory::FreeList::FreeList(size_t size) : _size(size) {
    LUG_ASSERT(_size > sizeof(lug::System::Memory::FreeList::Element*), "The size is not enough for this implementation of freelist");
}

bool lug::System::Memory::FreeList::grow(void* start, void *end, size_t alignment, size_t offset) {
    // First, align the start pointer
    {
        start = static_cast<char*>(start) + offset;

        if (start > end) {
            return false;
        }

        size_t size = static_cast<char*>(end) - static_cast<char*>(start) + 1;
        if (!std::align(alignment, _size - offset, start, size)) {
            return false;
        }

        start = static_cast<char*>(start) - offset;
    }

    // Second create the linked list
    {
        Element *it = static_cast<Element*>(start);
        _nextFree = it;

        size_t size = static_cast<char*>(end) - static_cast<char*>(start) + 1;
        for (size_t i = 1, count = size / _size; i < count; i++) {
            void* const next = static_cast<char*>(static_cast<void*>(it)) + _size;

            it->next = static_cast<Element*>(next);
            it = static_cast<Element*>(next);
        }

        it->next = nullptr;
    }

    return true;
}

void* lug::System::Memory::FreeList::allocate() {
    if (!_nextFree) {
        return nullptr;
    }

    Element* const head = _nextFree;
    _nextFree = _nextFree->next;
    return head;
}

void lug::System::Memory::FreeList::free(void *ptr) {
    Element* const head = static_cast<Element*>(ptr);
    head->next = _nextFree;
    _nextFree = head;
}

void lug::System::Memory::FreeList::reset() {
    _nextFree = nullptr;
}
