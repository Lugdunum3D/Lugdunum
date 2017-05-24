#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandPool;
namespace Builder {
class CommandBuffer;
} // Builder

class Buffer;
class DescriptorSet;
class Framebuffer;
class PipelineLayout;
class RenderPass;

class LUG_GRAPHICS_API CommandBuffer {
    friend class Builder::CommandBuffer;

public:
    CommandBuffer() = default;

    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer(CommandBuffer&& CommandBuffer);

    CommandBuffer& operator=(const CommandBuffer&) = delete;
    CommandBuffer& operator=(CommandBuffer&& CommandBuffer);

    ~CommandBuffer();

    explicit operator VkCommandBuffer() const {
        return _commandBuffer;
    }

    const CommandPool* getCommandPool() const;

    // Add begin, end, etc
    bool begin(VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) const;
    bool end() const;

    #include <lug/Graphics/Vulkan/API/CommandBuffer/Buffer.inl>
    #include <lug/Graphics/Vulkan/API/CommandBuffer/DescriptorSet.inl>
    #include <lug/Graphics/Vulkan/API/CommandBuffer/RenderPass.inl>

    bool reset(bool releaseRessources = false) const;
    void destroy();

private:
    explicit CommandBuffer(VkCommandBuffer commandBuffer, const CommandPool* commandPool);

private:
    VkCommandBuffer _commandBuffer{VK_NULL_HANDLE};
    const CommandPool* _commandPool{nullptr};
};

#include <lug/Graphics/Vulkan/API/CommandBuffer.inl>

} // API
} // Vulkan
} // Graphics
} // lug
