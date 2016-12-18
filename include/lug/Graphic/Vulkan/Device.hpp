#pragma once

#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {

class LUG_GRAPHIC_API Device {
public:
    explicit Device(VkDevice device = VK_NULL_HANDLE);

    Device(const Device&) = delete;
    Device(Device&& device);

    Device& operator=(const Device&) = delete;
    Device& operator=(Device&& device);

    ~Device();

    operator VkDevice() const {
        return _device;
    }

    template <typename Function>
    Function getProcAddr(const char* name);

    void destroy();

private:
    VkDevice _device{VK_NULL_HANDLE};
};

#include <lug/Graphic/Vulkan/Device.inl>

} // Vulkan
} // Graphic
} // lug
