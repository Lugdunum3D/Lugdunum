inline void Buffer::setSize(VkDeviceSize size) {
    _size = size;
}

inline void Buffer::setFlags(VkBufferCreateFlags flags) {
    _flags = flags;
}

inline void Buffer::setUsage(VkBufferUsageFlags usage) {
    _usage = usage;
}

inline void Buffer::setExclusive(bool exclusive) {
    _exclusive = exclusive;
}

inline void Buffer::setQueueFamilyIndices(std::set<uint32_t> queueFamilyIndices) {
    _queueFamilyIndices = queueFamilyIndices;
}
