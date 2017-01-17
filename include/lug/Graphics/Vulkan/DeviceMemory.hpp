#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <memory>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API DeviceMemory {
public:
    DeviceMemory(VkDeviceMemory deviceMemory = VK_NULL_HANDLE, const Device* device = nullptr);

    DeviceMemory(const DeviceMemory&) = delete;
    DeviceMemory(DeviceMemory&& deviceMemory);

    DeviceMemory& operator=(const DeviceMemory&) = delete;
    DeviceMemory& operator=(DeviceMemory&& deviceMemory);

    ~DeviceMemory();

    operator VkDeviceMemory() const {
        return _deviceMemory;
    }

    void destroy();

    static std::unique_ptr<DeviceMemory> allocate(const Device* device, VkDeviceSize size, VkDeviceSize memoryTypeIndex);
    static uint32_t findMemoryType(const Device* device, VkMemoryRequirements& memoryRequirements);

private:
    VkDeviceMemory _deviceMemory{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
