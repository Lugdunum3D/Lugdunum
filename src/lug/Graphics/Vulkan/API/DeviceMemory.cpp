#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

DeviceMemory::DeviceMemory(VkDeviceMemory deviceMemory, const Device* device, VkDeviceSize size) : _deviceMemory(deviceMemory), _device(device), _size(size) {}

DeviceMemory::DeviceMemory(DeviceMemory&& deviceMemory) {
    _deviceMemory = deviceMemory._deviceMemory;
    _device = deviceMemory._device;
    _size = deviceMemory._size;
    deviceMemory._deviceMemory = VK_NULL_HANDLE;
    deviceMemory._device = nullptr;
    deviceMemory._size = 0;
}

DeviceMemory& DeviceMemory::operator=(DeviceMemory&& deviceMemory) {
    destroy();

    _deviceMemory = deviceMemory._deviceMemory;
    _device = deviceMemory._device;
    _size = deviceMemory._size;
    deviceMemory._deviceMemory = VK_NULL_HANDLE;
    deviceMemory._device = nullptr;
    deviceMemory._size = 0;

    return *this;
}

DeviceMemory::~DeviceMemory() {
    destroy();
}

void DeviceMemory::destroy() {
    if (_deviceMemory != VK_NULL_HANDLE) {
        vkFreeMemory(static_cast<VkDevice>(*_device), _deviceMemory, nullptr);
        _deviceMemory = VK_NULL_HANDLE;
    }
}

void* DeviceMemory::map(VkDeviceSize size, VkDeviceSize offset) const {
    void* data = nullptr;

    VkResult result = vkMapMemory(static_cast<VkDevice>(*_device), _deviceMemory, offset, size, 0, &data);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("DeviceMemory: Can't map memory: {}", result);
        return nullptr;
    }

    return data;
}

void* DeviceMemory::mapBuffer(const API::Buffer& buffer, VkDeviceSize size, VkDeviceSize offset) const {
    if (buffer.getDeviceMemory() != this) {
        LUG_LOG.error("DeviceMemory: Can't map memory of a buffer: The buffer use a different device memory");
        return nullptr;
    }

    void* data = nullptr;

    VkResult result = vkMapMemory(
        static_cast<VkDevice>(*_device),
        _deviceMemory,
        offset + buffer.getDeviceMemoryOffset(),
        size == VK_WHOLE_SIZE ? buffer.getRequirements().size - offset : size,
        0,
        &data
    );

    if (result != VK_SUCCESS) {
        LUG_LOG.error("DeviceMemory: Can't map memory of a buffer: {}", result);
        return nullptr;
    }

    return data;
}

void DeviceMemory::unmap() const {
    vkUnmapMemory(static_cast<VkDevice>(*_device), _deviceMemory);
}

} // API
} // Vulkan
} // Graphics
} // lug
