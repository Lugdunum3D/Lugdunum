inline void PipelineLayout::setPushConstants(const std::vector<VkPushConstantRange>& pushConstants) {
    _pushConstants = pushConstants;
}

inline void PipelineLayout::setDescriptorSetLayouts(std::vector<API::DescriptorSetLayout> descriptorSetLayouts) {
    _descriptorSetLayouts = std::move(descriptorSetLayouts);
}
