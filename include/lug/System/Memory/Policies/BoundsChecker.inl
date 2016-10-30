inline void NoBoundsChecking::guardFront(void* ptr, size_t size) const {}
inline void NoBoundsChecking::guardBack(void* ptr, size_t size) const {}

inline void NoBoundsChecking::checkFront(void* ptr, size_t size) const {}
inline void NoBoundsChecking::checkBack(void* ptr, size_t size) const {}

inline void SimpleBoundsChecking::guardFront(void* ptr, size_t size) const {
    std::memcpy(ptr, SimpleBoundsChecking::MagicFront, SimpleBoundsChecking::SizeFront);
}

inline void SimpleBoundsChecking::guardBack(void* ptr, size_t size) const {
    std::memcpy(static_cast<char*>(ptr) + size - SimpleBoundsChecking::SizeBack, SimpleBoundsChecking::MagicBack, SimpleBoundsChecking::SizeBack);
}

inline void SimpleBoundsChecking::checkFront(void* ptr, size_t size) const {
    LUG_ASSERT(
        std::memcmp(ptr, SimpleBoundsChecking::MagicFront, SimpleBoundsChecking::SizeFront) == 0,
        "Memory overwrite at front of the user buffer"
    );
}

inline void SimpleBoundsChecking::checkBack(void* ptr, size_t size) const {
    LUG_ASSERT(
        std::memcmp(static_cast<char*>(ptr) + size - SimpleBoundsChecking::SizeBack, SimpleBoundsChecking::MagicBack, SimpleBoundsChecking::SizeBack) == 0,
        "Memory overwrite at back of the user buffer"
    );
}
