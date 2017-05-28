#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandBuffer;

class LUG_GRAPHICS_API Queue {
public:
    explicit Queue(VkQueue queue = VK_NULL_HANDLE);

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

private:
    VkQueue _queue{VK_NULL_HANDLE};
};

} // API
} // Vulkan
} // Graphics
} // lug
