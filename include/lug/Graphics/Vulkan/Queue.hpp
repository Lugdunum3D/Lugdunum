#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/CommandPool.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Queue {
public:
    explicit Queue(int8_t idx = -1, VkQueue queue = VK_NULL_HANDLE, VkQueueFlags flags = 0, bool presentation = false);

    Queue(const Queue&) = delete;
    Queue(Queue&& queue);

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&& queue);

    ~Queue();

    explicit operator VkQueue() const {
        return _queue;
    }

    int8_t getFamilyIdx() const;
    VkQueueFlags getFlags() const;

    bool supportsPresentation() const;
    void supportsPresentation(bool presentation);

    CommandPool& getCommandPool();
    const CommandPool& getCommandPool() const;

    void setCommandPool(CommandPool&& commandPool);
    bool submit(const CommandBuffer& commandBuffer,
                const std::vector<VkSemaphore>& signalSemaphores = {},
                const std::vector<VkSemaphore>& waitSemaphores = {},
                const std::vector<VkPipelineStageFlags>& waitDstStageMasks = {},
                VkFence fence = VK_NULL_HANDLE) const;
    bool waitIdle() const;

    void destroy();

private:
    int8_t _idx{-1};
    VkQueue _queue{VK_NULL_HANDLE};
    VkQueueFlags _flags{0};
    bool _presentation{false};

    CommandPool _commandPool{VK_NULL_HANDLE, nullptr, this};
};

#include <lug/Graphics/Vulkan/Queue.inl>

} // Vulkan
} // Graphics
} // lug
