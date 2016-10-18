template<size_t MaxSize, size_t MaxAlignment, size_t Offset>
Chunk<MaxSize, MaxAlignment, Offset>::Chunk(lug::System::Memory::Area::IArea* area) : _area{area}, _currentPage{_area->requestNextPage()}, _firstPage{_currentPage} {}

template<size_t MaxSize, size_t MaxAlignment, size_t Offset>
void* Chunk<MaxSize, MaxAlignment, Offset>::allocate(size_t size, size_t alignment, size_t offset) {
    LUG_ASSERT(offset == Offset, "Chunk allocator doesn't support multiple offset");
    LUG_ASSERT(MaxSize >= size, "Size of the allocation is greater than the chunk max size");
    LUG_ASSERT(MaxAlignment >= alignment, "Alignment of the allocation is greater than the chunk max alignment");

    void* const ptr = _freeList.allocate();

    if (ptr) {
        return ptr;
    }

    if (!_currentPage || !_freeList.grow(_currentPage->start, _currentPage->end, MaxAlignment, Offset)) {
        return nullptr;
    }

    _currentPage = _currentPage->next = (_currentPage->next ? _currentPage->next : _area->requestNextPage());
    return allocate(size, alignment, offset);
}

template<size_t MaxSize, size_t MaxAlignment, size_t Offset>
void Chunk<MaxSize, MaxAlignment, Offset>::free(void* ptr) {
    _freeList.free(ptr);
}

template<size_t MaxSize, size_t MaxAlignment, size_t Offset>
void Chunk<MaxSize, MaxAlignment, Offset>::reset() {
    _freeList.reset();
    _currentPage = _firstPage;
}

template<size_t MaxSize, size_t MaxAlignment, size_t Offset>
size_t Chunk<MaxSize, MaxAlignment, Offset>::getSize(void* ptr) const {
    return Chunk<MaxSize, MaxAlignment>::ChunkSize;
}
