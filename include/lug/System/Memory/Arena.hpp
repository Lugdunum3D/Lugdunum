#pragma once

#include <cstdlib>
#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {

template <
    class Allocator,
    class ThreadPolicy,
    class BoundsCheckingPolicy,
    class MemoryMarkingPolicy
>
class Arena {
public:
    Arena() = default;
    explicit Arena(Area::IArea* area);

    Arena(const Arena&) = delete;
    Arena(Arena&&) = default;

    Arena& operator=(const Arena&) = delete;
    Arena& operator=(Arena&&) = default;

    ~Arena() = default;

    void* allocate(size_t size, size_t alignment, size_t offset, const char* file, size_t line);
    void free(void* ptr);
    void reset();

    Allocator& allocator();
    const Allocator& allocator() const;

private:
    Allocator _allocator;
    ThreadPolicy _threadGuard;
    BoundsCheckingPolicy _boundsChecker;
    MemoryMarkingPolicy _memoryMarker;
};

#include <lug/System/Memory/Arena.inl>

} // Memory
} // Sytem
} // lug
