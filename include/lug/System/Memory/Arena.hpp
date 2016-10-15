#pragma once

#include <cstdlib>
#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {

template<class Allocator, class ThreadPolicy, class BoundsCheckingPolicy>
class LUG_SYSTEM_API Arena {
public:
    Arena() = default;
    explicit Arena(lug::System::Memory::Area::IArea* area);

    Arena(const Arena&) = delete;
    Arena(Arena&&) = default;

    Arena& operator=(const Arena&) = delete;
    Arena& operator=(Arena&&) = default;

    ~Arena() = default;

    void* allocate(size_t size, size_t alignment, const char* file, size_t line);
    void free(void* ptr);
    void reset();

private:
    Allocator _allocator;
    ThreadPolicy _threadGuard;
    BoundsCheckingPolicy _boundsChecker;
};

#include <lug/System/Memory/Arena.inl>

}
}
}
