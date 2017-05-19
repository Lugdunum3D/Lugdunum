#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
    class DeviceMemory;
};

class Device;

class LUG_GRAPHICS_API DeviceMemory {
    friend class Builder::DeviceMemory;

public:
    DeviceMemory() = default;

    DeviceMemory(const DeviceMemory&) = delete;
    DeviceMemory(DeviceMemory&& deviceMemory);

    DeviceMemory& operator=(const DeviceMemory&) = delete;
    DeviceMemory& operator=(DeviceMemory&& deviceMemory);

    ~DeviceMemory();

    explicit operator VkDeviceMemory() const {
        return _deviceMemory;
    }

    void destroy();

    static uint32_t findMemoryType(const Device* device, uint32_t memoryTypeBits, VkMemoryPropertyFlags requiredFlags);

private:
    explicit DeviceMemory(VkDeviceMemory deviceMemory, const Device* device);

private:
    VkDeviceMemory _deviceMemory{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
