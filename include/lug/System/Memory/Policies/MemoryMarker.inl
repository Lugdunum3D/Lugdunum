inline void NoMemoryMarking::markAllocation(void*, size_t) const {}
inline void NoMemoryMarking::markDeallocation(void*, size_t) const {}

inline void SimpleMemoryMarking::markAllocation(void* ptr, size_t size) const {
    std::memset(ptr, SimpleMemoryMarking::AllocationMagic, size);
}

inline void SimpleMemoryMarking::markDeallocation(void* ptr, size_t size) const {
    std::memset(ptr, SimpleMemoryMarking::DeallocationMagic, size);
}
