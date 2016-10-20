template <size_t Size>
inline Page* lug::System::Memory::Area::Stack<Size>::requestNextPage() {
    if (_nextPage) {
        _nextPage = false;
        return &_page;
    }

    return nullptr;
}
