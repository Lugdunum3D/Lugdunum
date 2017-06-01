struct CmdPipelineBarrier {
    struct MemoryBarrier {
        VkAccessFlags srcAccessMask;
        VkAccessFlags dstAccessMask;
    };

    struct BufferMemoryBarrier {
        VkAccessFlags srcAccessMask;
        VkAccessFlags dstAccessMask;
        uint32_t srcQueueFamilyIndex{VK_QUEUE_FAMILY_IGNORED};
        uint32_t dstQueueFamilyIndex{VK_QUEUE_FAMILY_IGNORED};
        const API::Buffer* buffer{nullptr};
        VkDeviceSize offset{0};
        VkDeviceSize size{VK_WHOLE_SIZE};
    };

    // We define an abstraction of VkImageSubresourceRange
    // because we will rarely use more than 1 field (and want others to be defaulted)
    struct ImageMemoryBarrier {
        // Move in API::Image ?
        struct ImageSubresourceRange {
            VkImageAspectFlags aspectMask{VK_IMAGE_ASPECT_COLOR_BIT};
            uint32_t baseMipLevel{0};
            uint32_t levelCount{1};
            uint32_t baseArrayLayer{0};
            uint32_t layerCount{1};
        };

        // The image src/access masks can be guessed with the new old/new layouts
        // We can either set manually the src/access masks or let it be chosen automatically
        //
        // We use 0xFFFFFFFF to know if no flag is set
        // 0x0 cannot be used because it's allowed for some commands
        VkAccessFlags srcAccessMask{0xFFFFFFFF};
        VkAccessFlags dstAccessMask{0xFFFFFFFF};
        VkImageLayout oldLayout{VK_IMAGE_LAYOUT_UNDEFINED};
        VkImageLayout newLayout{VK_IMAGE_LAYOUT_UNDEFINED};
        uint32_t srcQueueFamilyIndex{VK_QUEUE_FAMILY_IGNORED};
        uint32_t dstQueueFamilyIndex{VK_QUEUE_FAMILY_IGNORED};
        const API::Image* image{nullptr};
        ImageSubresourceRange subresourceRange;
    };

    std::vector<MemoryBarrier> memoryBarriers;
    std::vector<ImageMemoryBarrier> imageMemoryBarriers;
    std::vector<BufferMemoryBarrier> bufferMemoryBarriers;
};

struct CmdPushConstants {
    VkPipelineLayout layout = VK_NULL_HANDLE;
    VkShaderStageFlags stageFlags = VK_SHADER_STAGE_ALL;
    uint32_t offset = 0;
    uint32_t size = 0;
    const void* values = nullptr;
};

void pipelineBarrier(
    const CmdPipelineBarrier& parameters,
    VkDependencyFlags dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
) const;

void bindPipeline(const API::GraphicsPipeline& pipeline, VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS) const;
void bindVertexBuffers(
    const std::vector<const API::Buffer*>& buffers,
    const std::vector<VkDeviceSize>& offsets,
    uint32_t firstBinding = 0
) const;
void bindIndexBuffer(const API::Buffer& buffer, VkIndexType indexType, VkDeviceSize offset = 0) const;

void setViewport(const std::vector<VkViewport>& viewports, uint32_t firstViewport = 0) const;
void setScissor(const std::vector<VkRect2D>& scissors, uint32_t firstScissor = 0) const;
void setBlendConstants(const float blendConstants[4]) const;

void pushConstants(const CmdPushConstants& parameters) const;
