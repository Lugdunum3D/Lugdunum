template <size_t PageSize, size_t PageCount>
lug::System::Memory::Area::Heap<PageSize, PageCount>::Heap() {
    _data = std::malloc(PageSize * PageCount);

    char* const tmpPtr = static_cast<char*>(_data);
    for (size_t i = 0; i < PageCount; ++i) {
        _pages[i] = {
            tmpPtr + PageSize * i,
            tmpPtr + PageSize * (i + 1) - 1,
            i == 0 ? nullptr : &_pages[i - 1],
            nullptr
        };
    }
}

template <size_t PageSize, size_t PageCount>
lug::System::Memory::Area::Heap<PageSize, PageCount>::~Heap() {
    std::free(_data);
    _data = nullptr;
}

template <size_t PageSize, size_t PageCount>
inline Page* lug::System::Memory::Area::Heap<PageSize, PageCount>::requestNextPage() {
    if (_current >= PageCount) {
        return nullptr;
    }

    _current += 1;
    return &_pages[_current - 1];
}
