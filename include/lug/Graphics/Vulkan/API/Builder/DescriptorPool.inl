inline void DescriptorPool::setFlags(VkDescriptorPoolCreateFlags flags) {
    _flags = flags;
}

inline void DescriptorPool::setMaxSets(uint32_t maxSets) {
    _maxSets = maxSets;
}

inline void DescriptorPool::setPoolSizes(const std::vector<VkDescriptorPoolSize>& poolSizes) {
    _poolSizes = poolSizes;
}
