#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Device {
public:
    explicit Device(VkDevice device = VK_NULL_HANDLE, PhysicalDeviceInfo* physicalDeviceInfo = {});

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
    VkDevice _device{VK_NULL_HANDLE};
    PhysicalDeviceInfo* _physicalDeviceInfo{nullptr};
};

#include <lug/Graphics/Vulkan/Device.inl>

} // Vulkan
} // Graphics
} // lug
