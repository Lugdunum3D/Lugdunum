#pragma once

#include <memory>
#include <string>
#include <vector>

#include <lug/Core/Version.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

class Device {
private:
    struct QueueCreateInfo {
        int8_t queueFamilyIndex;
        std::vector<float> queuePriorities;
    };

public:
    Device(const PhysicalDeviceInfo& physicalDeviceInfo);

    Device(const Device&&) = delete;
    Device(Device&&) = delete;

    Device& operator=(const Device&&) = delete;
    Device& operator=(Device&&) = delete;

    ~Device() = default;

    // Setters
    void setExtensions(std::vector<const char*> extensions);
    void setFeatures(VkPhysicalDeviceFeatures features);

    int8_t addQueue(VkQueueFlags queueFlags, uint8_t queueCount);

    // Build methods
    bool build(API::Device& device, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Device> build(VkResult* returnResult = nullptr);

private:
    const PhysicalDeviceInfo& _physicalDeviceInfo;

    // We don't have layers because it's deprecated
    std::vector<const char*> _extensions;
    VkPhysicalDeviceFeatures _features{};

    std::vector<QueueCreateInfo> _queueCreateInfos;
};

#include <lug/Graphics/Vulkan/API/Builder/Device.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
