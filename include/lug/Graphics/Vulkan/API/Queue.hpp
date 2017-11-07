#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <mutex>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandBuffer;
class QueueFamily;

class LUG_GRAPHICS_API Queue {
public:
    explicit Queue(VkQueue queue = VK_NULL_HANDLE, const QueueFamily* queueFamily = nullptr);

    Queue(const Queue&) = delete;
    Queue(Queue&& queue);

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&& queue);

    ~Queue() = default;

    explicit operator VkQueue() const {
        return _queue;
    }

    bool submit(const CommandBuffer& commandBuffer,
                const std::vector<VkSemaphore>& signalSemaphores = {},
                const std::vector<VkSemaphore>& waitSemaphores = {},
                const std::vector<VkPipelineStageFlags>& waitDstStageMasks = {},
                VkFence fence = VK_NULL_HANDLE
    ) const;

    bool waitIdle() const;

    const QueueFamily* getQueueFamily() const;

private:
    VkQueue _queue{VK_NULL_HANDLE};

    const QueueFamily* _queueFamily{nullptr};
    mutable std::mutex _mutex;
};

} // API
} // Vulkan
} // Graphics
} // lug
