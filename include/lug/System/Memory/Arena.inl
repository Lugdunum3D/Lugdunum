template<class Allocator, class ThreadPolicy, class BoundsCheckingPolicy>
Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy>::Arena(lug::System::Memory::Area::IArea* area) : _allocator{area} {}

template<class Allocator, class ThreadPolicy, class BoundsCheckingPolicy>
void* Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy>::allocate(size_t size, size_t alignment, const char* file, size_t line) {
    const size_t newSize = size + BoundsCheckingPolicy::SizeFront + BoundsCheckingPolicy::SizeBack;

    _threadGuard.enter();

    char* const ptr = static_cast<char*>(_allocator.allocate(newSize, alignment, BoundsCheckingPolicy::SizeFront));
    const size_t allocatedSize = _allocator.getSize(ptr);

    _boundsChecker.guardFront(ptr, allocatedSize);
    _boundsChecker.guardBack(ptr, allocatedSize);

    _threadGuard.leave();

    return (ptr + BoundsCheckingPolicy::SizeFront);
}

template<class Allocator, class ThreadPolicy, class BoundsCheckingPolicy>
void Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy>::free(void* ptr) {
    if (!ptr) {
        return;
    }

    char* const originalMemory = static_cast<char*>(ptr) - BoundsCheckingPolicy::SizeFront;
    const size_t allocatedSize = _allocator.getSize(originalMemory);

    _threadGuard.enter();

    _boundsChecker.checkFront(originalMemory, allocatedSize);
    _boundsChecker.checkBack(originalMemory, allocatedSize);

    _allocator.free(originalMemory);

    _threadGuard.leave();
}

template<class Allocator, class ThreadPolicy, class BoundsCheckingPolicy>
void Arena<Allocator, ThreadPolicy, BoundsCheckingPolicy>::reset() {
    _threadGuard.enter();

    _boundsChecker.checkReset();
    _allocator.reset();

    _threadGuard.leave();
}
