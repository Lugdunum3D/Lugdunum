#pragma once

#include <lug/Graphics/Export.hpp>
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

    void destroy();

    const PhysicalDeviceInfo* getPhysicalDeviceInfo() const;

private:
    explicit Device(VkDevice device, const PhysicalDeviceInfo* physicalDeviceInfo);

private:
    VkDevice _device{VK_NULL_HANDLE};
    const PhysicalDeviceInfo* _physicalDeviceInfo{nullptr};
};

#include <lug/Graphics/Vulkan/API/Device.inl>

} // API
} // Vulkan
} // Graphics
} // lug
