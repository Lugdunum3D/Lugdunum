inline void Device::setExtensions(std::vector<const char*> extensions) {
    _extensions = extensions;
}

inline void Device::setFeatures(VkPhysicalDeviceFeatures features) {
    _features = features;
}
