struct CmdBindDescriptors {
    const API::PipelineLayout& pipelineLayout;
    VkPipelineBindPoint pipelineBindPoint{VK_PIPELINE_BIND_POINT_GRAPHICS};
    uint32_t firstSet{0};
    std::vector<const API::DescriptorSet*> descriptorSets;
    std::vector<uint32_t> dynamicOffsets;
};

void bindDescriptorSets(const CmdBindDescriptors& parameters) const;
