inline void Image::setCreateFlags(VkImageCreateFlags createFlags) {
    _createFlags = createFlags;
}

inline void Image::setImageType(VkImageType imageType) {
    _imageType = imageType;
}

inline void Image::setPreferedFormats(const std::set<VkFormat>& preferedFormats) {
    _preferedFormats = preferedFormats;
}

inline void Image::setFeatureFlags(VkFormatFeatureFlags featureFlags) {
    _featureFlags = featureFlags;
}

inline void Image::setExtent(VkExtent3D extent) {
    _extent = extent;
}

inline void Image::setMipLevels(uint32_t mipLevels) {
    _mipLevels = mipLevels;
}

inline void Image::setArrayLayers(uint32_t arrayLayers) {
    _arrayLayers = arrayLayers;
}

inline void Image::setSampleCount(VkSampleCountFlagBits sampleCount) {
    _sampleCount = sampleCount;
}

inline void Image::setTiling(VkImageTiling tiling) {
    _tiling = tiling;
}

inline void Image::setUsage(VkImageUsageFlags usage) {
    _usage = usage;
}

inline void Image::setExclusive(bool exclusive) {
    _exclusive = exclusive;
}

inline void Image::setQueueFamilyIndices(const std::set<uint32_t>& queueFamilyIndices) {
    _queueFamilyIndices = queueFamilyIndices;
}
