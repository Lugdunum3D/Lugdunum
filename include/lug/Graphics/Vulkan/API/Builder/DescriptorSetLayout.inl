inline void DescriptorSetLayout::setBindings(std::vector<VkDescriptorSetLayoutBinding> bindings) {
    _bindings = std::move(bindings);
}
