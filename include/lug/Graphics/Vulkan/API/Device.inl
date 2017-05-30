template <typename Function>
inline Function Device::getProcAddr(const char* name) const {
    return reinterpret_cast<Function>(vkGetDeviceProcAddr(_device, name));
}

inline const PhysicalDeviceInfo* Device::getPhysicalDeviceInfo() const {
    return _physicalDeviceInfo;
}

inline const std::vector<QueueFamily>& Device::getQueueFamilies() const {
    return _queueFamilies;
}

inline std::vector<QueueFamily>& Device::getQueueFamilies() {
    return _queueFamilies;
}
