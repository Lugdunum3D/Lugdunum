#include <lug/Graphics/Vulkan/API/QueueFamily.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

QueueFamily::QueueFamily(uint32_t idx, VkQueueFlags flags) : _idx(idx), _flags(flags) {}

QueueFamily::QueueFamily(QueueFamily&& queue) {
    _idx = queue._idx;
    _flags = queue._flags;
    _queues = std::move(queue._queues);
    _queuesIndices = std::move(queue._queuesIndices);

    queue._idx = 0;
    queue._flags = 0;
}

QueueFamily& QueueFamily::operator=(QueueFamily&& queue) {
    _idx = queue._idx;
    _flags = queue._flags;
    _queues = std::move(queue._queues);
    _queuesIndices = std::move(queue._queuesIndices);

    queue._idx = 0;
    queue._flags = 0;

    return *this;
}

} // API
} // Vulkan
} // Graphics
} // lug
