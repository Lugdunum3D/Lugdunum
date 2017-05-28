#pragma once

#include <memory>
#include <string>
#include <vector>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

class Device {
private:
    struct Queue {
        VkQueueFlags flagsUsed{};
        std::vector<std::string> names;
    };

    struct QueueFamily {
        VkQueueFlags flags{};
        uint32_t queuesUsed{0};
        std::vector<Queue> queues;
    };

public:
    Device(const PhysicalDeviceInfo& physicalDeviceInfo);

    Device(const Device&&) = delete;
    Device(Device&&) = delete;

    Device& operator=(const Device&&) = delete;
    Device& operator=(Device&&) = delete;

    ~Device() = default;

    // Setters
    void setExtensions(const std::vector<const char*>& extensions);
    void setFeatures(VkPhysicalDeviceFeatures features);

    uint8_t addQueues(VkQueueFlags queueFlags, const std::vector<std::string>& queuesNames);

    // Build methods
    bool build(API::Device& device, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Device> build(VkResult* returnResult = nullptr);

private:
    bool addQueue(VkQueueFlags queueFlags, const std::string& queueName);

private:
    const PhysicalDeviceInfo& _physicalDeviceInfo;

    // We don't have layers because it's deprecated
    std::vector<const char*> _extensions;
    VkPhysicalDeviceFeatures _features{};

    std::vector<QueueFamily> _queueFamiliesInfos;
};

#include <lug/Graphics/Vulkan/API/Builder/Device.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
