inline const VkMemoryRequirements& Image::getRequirements() const {
    return _requirements;
}

inline const DeviceMemory* Image::getDeviceMemory() const {
    return _deviceMemory;
}

inline VkDeviceSize Image::getDeviceMemoryOffset() const {
    return _deviceMemoryOffset;
}

inline VkFormat Image::getFormat() const {
    return _format;
}