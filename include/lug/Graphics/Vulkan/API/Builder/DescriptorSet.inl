inline void DescriptorSet::setDescriptorSetLayouts(std::vector<VkDescriptorSetLayout> descriptorSetLayouts) {
    _descriptorSetLayouts = std::move(descriptorSetLayouts);
}
