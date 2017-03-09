#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

class LUG_GRAPHICS_API DeviceMemory {
public:
    explicit DeviceMemory(VkDeviceMemory deviceMemory = VK_NULL_HANDLE, const Device* device = nullptr);

    DeviceMemory(const DeviceMemory&) = delete;
    DeviceMemory(DeviceMemory&& deviceMemory);

    DeviceMemory& operator=(const DeviceMemory&) = delete;
    DeviceMemory& operator=(DeviceMemory&& deviceMemory);

    ~DeviceMemory();

    explicit operator VkDeviceMemory() const {
        return _deviceMemory;
    }

    void destroy();

    static std::unique_ptr<DeviceMemory> allocate(const Device* device, VkDeviceSize size, uint32_t memoryTypeIndex);
    static uint32_t findMemoryType(const Device* device, const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags requiredFlags);

private:
    VkDeviceMemory _deviceMemory{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
