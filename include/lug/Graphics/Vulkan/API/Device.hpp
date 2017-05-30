#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/QueueFamily.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Device;
} // Builder

class LUG_GRAPHICS_API Device {
    friend class Builder::Device;

public:
    Device() = default;

    Device(const Device&) = delete;
    Device(Device&& device);

    Device& operator=(const Device&) = delete;
    Device& operator=(Device&& device);

    ~Device();

    explicit operator VkDevice() const {
        return _device;
    }

    template <typename Function>
    Function getProcAddr(const char* name) const;

    const PhysicalDeviceInfo* getPhysicalDeviceInfo() const;

    const std::vector<QueueFamily>& getQueueFamilies() const;
    std::vector<QueueFamily>& getQueueFamilies();

    const API::QueueFamily* getQueueFamily(VkQueueFlags flags, bool supportPresentation = false) const;
    API::QueueFamily* getQueueFamily(VkQueueFlags flags, bool supportPresentation = false);

    bool waitIdle() const;

    void destroy();


private:
    explicit Device(VkDevice device, const PhysicalDeviceInfo* physicalDeviceInfo);

private:
    VkDevice _device{VK_NULL_HANDLE};

    const PhysicalDeviceInfo* _physicalDeviceInfo{nullptr};
    std::vector<QueueFamily> _queueFamilies;
};

#include <lug/Graphics/Vulkan/API/Device.inl>

} // API
} // Vulkan
} // Graphics
} // lug
