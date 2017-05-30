inline void Device::setExtensions(const std::vector<const char*>& extensions) {
    _extensions = extensions;
}

inline void Device::setFeatures(VkPhysicalDeviceFeatures features) {
    _features = features;
}
