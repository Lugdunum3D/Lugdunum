struct CmdBeginRenderPass{
    const API::Framebuffer& framebuffer;
    VkRect2D renderArea;
    std::vector<VkClearValue> clearValues;
};

void beginRenderPass(const API::RenderPass& renderPass, const CmdBeginRenderPass& parameters, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
void endRenderPass();
