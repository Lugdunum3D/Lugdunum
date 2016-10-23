template <size_t PageSize, size_t MaxPageCount>
lug::System::Memory::Area::GrowingHeap<PageSize, MaxPageCount>::GrowingHeap() {
    std::memset(_data, 0, sizeof(void*) * MaxPageCount);
}

template <size_t PageSize, size_t MaxPageCount>
lug::System::Memory::Area::GrowingHeap<PageSize, MaxPageCount>::~GrowingHeap() {
    for (size_t i = 0; i < MaxPageCount; ++i) {
        std::free(_data[i]);
        _data[i] = nullptr;
    }
}

template <size_t PageSize, size_t MaxPageCount>
inline Page* lug::System::Memory::Area::GrowingHeap<PageSize, MaxPageCount>::requestNextPage() {
    if (_current >= MaxPageCount) {
        return nullptr;
    }

    _data[_current] = std::malloc(PageSize);

    if (!_data[_current]) {
        return nullptr;
    }

    _pages[_current] = {
        _data[_current],
        static_cast<char*>(_data[_current]) + PageSize,
        _current == 0 ? nullptr : &_pages[_current - 1],
        nullptr
    };

    _current += 1;
    return &_pages[_current - 1];
}
