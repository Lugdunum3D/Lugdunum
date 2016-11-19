#include <lug/System/Memory/Allocator/Basic.hpp>
#include <malloc.h>
#include <lug/System/Debug.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Allocator {

void* Basic::allocate(size_t size, size_t alignment, size_t offset) const {
#if defined(LUG_SYSTEM_LINUX)
    return aligned_alloc(alignment + offset, size);
#elif defined(LUG_SYSTEM_ANDROID)
    void* ret = nullptr;
    if (!posix_memalign(&ret, alignment + offset, size)) {
        return ret;
    }
    return nullptr;
#elif defined(LUG_SYSTEM_WINDOWS)
    return _aligned_malloc(alignment + offset, size);
#endif
}

void Basic::free(void* ptr) const {
    std::free(ptr);
}

void Basic::reset() const {
    LUG_ASSERT(false, "Basic allocator doesn't implement reset method");
}

size_t Basic::getSize(void* ptr) const {
#if defined(LUG_SYSTEM_WINDOWS)
    return _msize(ptr);
#else
    return malloc_usable_size(ptr);
#endif
}


}
}
}
}
