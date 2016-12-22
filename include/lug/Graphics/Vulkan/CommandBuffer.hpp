#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class CommandPool;

class LUG_GRAPHICS_API CommandBuffer {
public:
    CommandBuffer(VkCommandBuffer commandBuffer = VK_NULL_HANDLE, CommandPool* commandPool = nullptr);

    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer(CommandBuffer&& CommandBuffer);

    CommandBuffer& operator=(const CommandBuffer&) = delete;
    CommandBuffer& operator=(CommandBuffer&& CommandBuffer);

    ~CommandBuffer();

    operator VkCommandBuffer() const {
        return _commandBuffer;
    }

    // Add begin, end, etc

    bool reset(bool releaseRessources = false);
    void destroy();

private:
    VkCommandBuffer _commandBuffer{VK_NULL_HANDLE};
    CommandPool* _commandPool{nullptr};
};

} // Vulkan
} // Graphics
} // lug
