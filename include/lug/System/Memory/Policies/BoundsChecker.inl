inline void NoBoundsChecking::guardFront(void*, size_t) const {}
inline void NoBoundsChecking::guardBack(void*, size_t) const {}

inline void NoBoundsChecking::checkFront(void*, size_t) const {}
inline void NoBoundsChecking::checkBack(void*, size_t) const {}

inline void SimpleBoundsChecking::guardFront(void* ptr, size_t) const {
    std::memcpy(ptr, SimpleBoundsChecking::MagicFront, SimpleBoundsChecking::SizeFront);
}

inline void SimpleBoundsChecking::guardBack(void* ptr, size_t size) const {
    std::memcpy(static_cast<char*>(ptr) + size - SimpleBoundsChecking::SizeBack, SimpleBoundsChecking::MagicBack, SimpleBoundsChecking::SizeBack);
}

inline void SimpleBoundsChecking::checkFront(void* ptr, size_t) const {
    // In release, LUG_ASSERT is discarded
    (void)(ptr);

    LUG_ASSERT(
        std::memcmp(ptr, SimpleBoundsChecking::MagicFront, SimpleBoundsChecking::SizeFront) == 0,
        "Memory overwrite at front of the user buffer"
    );
}

inline void SimpleBoundsChecking::checkBack(void* ptr, size_t size) const {
    // In release, LUG_ASSERT is discarded
    (void)(ptr);
    (void)(size);

    LUG_ASSERT(
        std::memcmp(static_cast<char*>(ptr) + size - SimpleBoundsChecking::SizeBack, SimpleBoundsChecking::MagicBack, SimpleBoundsChecking::SizeBack) == 0,
        "Memory overwrite at back of the user buffer"
    );
}
