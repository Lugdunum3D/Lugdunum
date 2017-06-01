#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class DeviceMemory;
} // Builder

class Buffer;
class Device;
class Image;

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

    void* map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
    void* mapBuffer(const API::Buffer& buffer, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
    void* mapImage(const API::Image& image, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

    void unmap() const;

    VkDeviceSize getSize() const;

private:
    explicit DeviceMemory(VkDeviceMemory deviceMemory, const Device* device, VkDeviceSize size);

private:
    VkDeviceMemory _deviceMemory{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    VkDeviceSize _size{0};
};

#include <lug/Graphics/Vulkan/API/DeviceMemory.inl>

} // API
} // Vulkan
} // Graphics
} // lug
