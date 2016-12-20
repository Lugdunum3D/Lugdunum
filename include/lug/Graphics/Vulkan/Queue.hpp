#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Queue {
public:
    Queue(int8_t idx = -1, VkQueue queue = VK_NULL_HANDLE, VkQueueFlags flags = 0);

    Queue(const Queue&) = delete;
    Queue(Queue&& queue);

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&& queue);

    ~Queue();

    operator VkQueue() const {
        return _queue;
    }

    int8_t getFamilyIdx() const;
    VkQueueFlags getFlags() const;

    void destroy();

private:
    int8_t _idx{-1};
    VkQueue _queue{VK_NULL_HANDLE};
    VkQueueFlags _flags{0};
};

#include <lug/Graphics/Vulkan/Queue.inl>

} // Vulkan
} // Graphics
} // lug
