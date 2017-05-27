inline const VkMemoryRequirements& Buffer::getRequirements() const {
    return _requirements;
}

inline const DeviceMemory* Buffer::getDeviceMemory() const {
    return _deviceMemory;
}

inline VkDeviceSize Buffer::getDeviceMemoryOffset() const {
    return _deviceMemoryOffset;
}
