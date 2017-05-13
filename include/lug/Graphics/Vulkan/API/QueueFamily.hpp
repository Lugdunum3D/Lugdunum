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

    explicit operator int8_t() const {
        return _idx;
    }

    int8_t getIdx() const;
    VkQueueFlags getFlags() const;

    const std::vector<Queue>& getQueues() const;
    const Queue* getQueue(const std::string& queueName) const;

    bool supportsPresentation() const;
    void supportsPresentation(bool presentation);

private:
    explicit QueueFamily(int8_t idx, VkQueueFlags flags);

private:
    int8_t _idx{-1};
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
