#include <lug/Graphics/Vulkan/Queue.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Queue::Queue(int8_t idx, VkQueue queue, VkQueueFlags flags, bool presentation) : _idx(idx), _queue(queue), _flags(flags), _presentation(presentation) {}

Queue::Queue(Queue&& queue) {
    _queue = queue._queue;
    _idx = queue._idx;
    _flags = queue._flags;
    _presentation = queue._presentation;

    queue._queue = VK_NULL_HANDLE;
    queue._idx = -1;
    queue._flags = 0;
    queue._presentation = false;
}

Queue& Queue::operator=(Queue&& queue) {
    _queue = queue._queue;
    _idx = queue._idx;
    _flags = queue._flags;
    _presentation = queue._presentation;

    queue._queue = VK_NULL_HANDLE;
    queue._idx = -1;
    queue._flags = 0;
    queue._presentation = false;

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
        _presentation = false;
    }
}

} // Vulkan
} // Graphics
} // lug
