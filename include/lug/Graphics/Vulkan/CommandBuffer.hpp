#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class CommandPool;

class LUG_GRAPHICS_API CommandBuffer {
public:
    explicit CommandBuffer(VkCommandBuffer commandBuffer = VK_NULL_HANDLE, CommandPool* commandPool = nullptr);

    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer(CommandBuffer&& CommandBuffer);

    CommandBuffer& operator=(const CommandBuffer&) = delete;
    CommandBuffer& operator=(CommandBuffer&& CommandBuffer);

    ~CommandBuffer();

    explicit operator VkCommandBuffer() const {
        return _commandBuffer;
    }

    // Add begin, end, etc
    bool begin(VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    bool end();

    bool reset(bool releaseRessources = false);
    void destroy();

private:
    VkCommandBuffer _commandBuffer{VK_NULL_HANDLE};
    CommandPool* _commandPool{nullptr};
};

} // Vulkan
} // Graphics
} // lug
