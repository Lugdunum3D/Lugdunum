struct CmdBeginRenderPass {
    const API::Framebuffer& framebuffer;
    VkRect2D renderArea;
    std::vector<VkClearValue> clearValues;
};

struct CmdDraw {
    uint32_t vertexCount = 0;
    uint32_t instanceCount = 0;
    uint32_t firstVertex = 0;
    uint32_t firstInstance = 0;
};

struct CmdDrawIndexed {
    uint32_t indexCount = 0;
    uint32_t instanceCount = 0;
    uint32_t firstIndex = 0;
    uint32_t vertexOffset = 0;
    uint32_t firstInstance = 0;
};

void beginRenderPass(const API::RenderPass& renderPass,
    const CmdBeginRenderPass& parameters,
    VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE
) const;
void endRenderPass() const;
void draw(const CmdDraw& params) const;
void drawIndexed(const CmdDrawIndexed& params) const;
