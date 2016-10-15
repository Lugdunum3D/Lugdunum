#include <lug/System/Memory/Allocator/Basic.hpp>
#include <malloc.h>
#include <lug/System/Debug.hpp>

void* lug::System::Memory::Allocator::Basic::allocate(size_t size, size_t alignment, size_t offset) const {
    return aligned_alloc(alignment + offset, size);
}

void lug::System::Memory::Allocator::Basic::free(void* ptr) const {
    std::free(ptr);
}

void lug::System::Memory::Allocator::Basic::reset() const {
    LUG_ASSERT(false, "Basic allocator doesn't implement reset method");
}

size_t lug::System::Memory::Allocator::Basic::getSize(void *ptr) const {
#if defined(LUG_SYSTEM_WINDOWS)
    return _msize(ptr);
#else
    return malloc_usable_size(ptr);
#endif
}
