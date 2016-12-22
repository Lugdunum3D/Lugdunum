#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Queue;

class LUG_GRAPHICS_API CommandPool {
public:
    CommandPool(VkCommandPool commandPool = VK_NULL_HANDLE, Device *device = nullptr, Queue *queue = nullptr);

    CommandPool(const CommandPool&) = delete;
    // Warning: Don't move CommandPool after creating a CommandBuffer
    CommandPool(CommandPool&& CommandPool);

    CommandPool& operator=(const CommandPool&) = delete;
    // Warning: Don't move CommandPool after creating a CommandBuffer
    CommandPool& operator=(CommandPool&& CommandPool);

    ~CommandPool();

    operator VkCommandPool() const {
        return _commandPool;
    }

    Device* getDevice() const;
    Queue* getQueue() const;

    // TODO: free
    std::vector<CommandBuffer> createCommandBuffers(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, uint32_t count = 1);

    bool reset(bool releaseRessources = false);

    // Warning: Command buffers created with this pool must have been destroyed
    void destroy();

private:
    VkCommandPool _commandPool{VK_NULL_HANDLE};
    Device* _device{nullptr};
    Queue* _queue{nullptr};
};

#include <lug/Graphics/Vulkan/CommandPool.inl>

} // Vulkan
} // Graphics
} // lug
