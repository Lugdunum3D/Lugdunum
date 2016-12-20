#include <lug/Graphics/Vulkan/Queue.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Queue::Queue(int8_t idx, VkQueue queue, VkQueueFlags flags) : _idx(idx), _queue(queue), _flags(flags) {}

Queue::Queue(Queue&& queue) {
    _queue = queue._queue;
    queue._queue = VK_NULL_HANDLE;
    queue._idx = -1;
    queue._flags = 0;
}

Queue& Queue::operator=(Queue&& queue) {
    _queue = queue._queue;
    queue._queue = VK_NULL_HANDLE;
    queue._idx = -1;
    queue._flags = 0;

    return *this;
}

Queue::~Queue() {
    destroy();
}

void Queue::destroy() {
    if (_queue != VK_NULL_HANDLE) {
        vkQueueWaitIdle(_queue);
        _queue = VK_NULL_HANDLE;
        _idx = -1;
        _flags = 0;
    }
}

} // Vulkan
} // Graphics
} // lug
