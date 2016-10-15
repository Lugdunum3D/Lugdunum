inline void NoBoundsChecking::guardFront(void* ptr, size_t size) const {}
inline void NoBoundsChecking::guardBack(void* ptr, size_t size) const {}

inline void NoBoundsChecking::checkFront(void* ptr, size_t size) const {}
inline void NoBoundsChecking::checkBack(void* ptr, size_t size) const {}
inline void NoBoundsChecking::checkReset() const {}
