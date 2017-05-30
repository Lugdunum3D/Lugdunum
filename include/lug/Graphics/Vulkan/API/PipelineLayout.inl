inline const std::vector<DescriptorSetLayout>& PipelineLayout::getDescriptorSetLayouts() const {
    return _descriptorSetLayouts;
}

inline const Device* PipelineLayout::getDevice() const {
    return _device;
}
