template <size_t size>
Page* lug::System::Memory::Area::Stack<size>::requestNextPage() {
    if (_nextPage) {
        _nextPage = false;
        return &_page;
    }

    return nullptr;
}
