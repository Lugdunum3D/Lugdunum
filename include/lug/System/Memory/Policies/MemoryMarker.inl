inline void NoMemoryMarking::markAllocation(void* ptr, size_t size) const {}
inline void NoMemoryMarking::markDeallocation(void* ptr, size_t size) const {}

inline void SimpleMemoryMarking::markAllocation(void* ptr, size_t size) const {
    std::memset(ptr, SimpleMemoryMarking::AllocationMagic, size);
}

inline void SimpleMemoryMarking::markDeallocation(void* ptr, size_t size) const {
    std::memset(ptr, SimpleMemoryMarking::DeallocationMagic, size);
}
