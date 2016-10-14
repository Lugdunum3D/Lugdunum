template<class Allocator>
Arena<Allocator>::Arena(lug::System::Memory::Area::IArea* area) : _allocator{area} {

}

template<class Allocator>
void* Arena<Allocator>::allocate(size_t size, size_t alignment, const char* file, int line) {
    return _allocator.allocate(size, alignment, 0);
}

template<class Allocator>
void Arena<Allocator>::free(void* ptr) {
    if (!ptr) {
        return;
    }

    return _allocator.free(ptr);
}

template<class Allocator>
void Arena<Allocator>::reset() {
    return _allocator.reset();
}
