template <
    class Allocator,
    class ThreadPolicy,
    class BoundsCheckingPolicy,
    class MemoryMarkingPolicy
>
Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy, MemoryMarkingPolicy>::Arena(Area::IArea* area) : _allocator{area} {}

template <
    class Allocator,
    class ThreadPolicy,
    class BoundsCheckingPolicy,
    class MemoryMarkingPolicy
>
void* Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy, MemoryMarkingPolicy>::allocate(size_t size, size_t alignment, size_t offset, const char* file, size_t line) {
    const size_t newSize = size + BoundsCheckingPolicy::SizeFront + BoundsCheckingPolicy::SizeBack;

    _threadGuard.enter();

    char* const ptr = static_cast<char*>(_allocator.allocate(newSize, alignment, offset + BoundsCheckingPolicy::SizeFront));
    const size_t allocatedSize = _allocator.getSize(ptr);

    _boundsChecker.guardFront(ptr, allocatedSize);
    _memoryMarker.markAllocation(ptr + BoundsCheckingPolicy::SizeFront, allocatedSize - BoundsCheckingPolicy::SizeFront - BoundsCheckingPolicy::SizeBack);
    _boundsChecker.guardBack(ptr, allocatedSize);

    _threadGuard.leave();

    return (ptr + BoundsCheckingPolicy::SizeFront);
}

template <
    class Allocator,
    class ThreadPolicy,
    class BoundsCheckingPolicy,
    class MemoryMarkingPolicy
>
void Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy, MemoryMarkingPolicy>::free(void* ptr) {
    if (!ptr) {
        return;
    }

    char* const originalMemory = static_cast<char*>(ptr) - BoundsCheckingPolicy::SizeFront;
    const size_t allocatedSize = _allocator.getSize(originalMemory);

    _threadGuard.enter();

    _boundsChecker.checkFront(originalMemory, allocatedSize);
    _boundsChecker.checkBack(originalMemory, allocatedSize);

    _memoryMarker.markDeallocation(originalMemory, allocatedSize);

    _allocator.free(originalMemory);

    _threadGuard.leave();
}

template <
    class Allocator,
    class ThreadPolicy,
    class BoundsCheckingPolicy,
    class MemoryMarkingPolicy
>
void Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy, MemoryMarkingPolicy>::reset() {
    _threadGuard.enter();

    _boundsChecker.checkReset();
    _allocator.reset();

    _threadGuard.leave();
}

template <
    class Allocator,
    class ThreadPolicy,
    class BoundsCheckingPolicy,
    class MemoryMarkingPolicy
>
Allocator& Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy, MemoryMarkingPolicy>::allocator() {
    return _allocator;
}

template <
    class Allocator,
    class ThreadPolicy,
    class BoundsCheckingPolicy,
    class MemoryMarkingPolicy
>
const Allocator& Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy, MemoryMarkingPolicy>::allocator() const {
    return _allocator;
}
