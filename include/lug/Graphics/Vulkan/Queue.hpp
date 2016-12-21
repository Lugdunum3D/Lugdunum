#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Queue {
public:
    Queue(int8_t idx = -1, VkQueue queue = VK_NULL_HANDLE, VkQueueFlags flags = 0, bool presentation = false);

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

    bool supportsPresentation() const;
    void supportsPresentation(bool presentation);

    void destroy();

private:
    int8_t _idx{-1};
    VkQueue _queue{VK_NULL_HANDLE};
    VkQueueFlags _flags{0};
    bool _presentation{false};
};

#include <lug/Graphics/Vulkan/Queue.inl>

} // Vulkan
} // Graphics
} // lug
