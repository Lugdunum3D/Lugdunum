#pragma once

#include <unordered_map>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/Queue.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Device;
} // Builder

class LUG_GRAPHICS_API QueueFamily {
    friend class Builder::Device;

public:
    QueueFamily() = default;

    QueueFamily(const QueueFamily&) = delete;
    QueueFamily(QueueFamily&& queueFamily);

    QueueFamily& operator=(const QueueFamily&) = delete;
    QueueFamily& operator=(QueueFamily&& queueFamily);

    ~QueueFamily() = default;

    explicit operator uint32_t() const {
        return _idx;
    }

    uint32_t getIdx() const;
    VkQueueFlags getFlags() const;

    const std::vector<Queue>& getQueues() const;
    const Queue* getQueue(const std::string& queueName) const;

    bool supportsPresentation() const;
    void supportsPresentation(bool presentation);

private:
    explicit QueueFamily(uint32_t idx, VkQueueFlags flags);

private:
    uint32_t _idx{0};
    bool _presentation{false};
    VkQueueFlags _flags{0};

    std::vector<Queue> _queues;
    std::unordered_map<std::string, uint32_t>  _queuesIndices;
};

#include <lug/Graphics/Vulkan/API/QueueFamily.inl>

} // API
} // Vulkan
} // Graphics
} // lug
