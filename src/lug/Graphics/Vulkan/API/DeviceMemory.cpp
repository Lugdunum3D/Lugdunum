#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

DeviceMemory::DeviceMemory(VkDeviceMemory deviceMemory, const Device* device) : _deviceMemory(deviceMemory), _device(device) {}

DeviceMemory::DeviceMemory(DeviceMemory&& deviceMemory) {
    _deviceMemory = deviceMemory._deviceMemory;
    _device = deviceMemory._device;
    deviceMemory._deviceMemory = VK_NULL_HANDLE;
    deviceMemory._device = nullptr;
}

DeviceMemory& DeviceMemory::operator=(DeviceMemory&& deviceMemory) {
    destroy();

    _deviceMemory = deviceMemory._deviceMemory;
    _device = deviceMemory._device;
    deviceMemory._deviceMemory = VK_NULL_HANDLE;
    deviceMemory._device = nullptr;

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

uint32_t DeviceMemory::findMemoryType(const Device* device, uint32_t memoryTypeBits, VkMemoryPropertyFlags requiredFlags) {
    const PhysicalDeviceInfo* physicalDeviceInfo = device->getPhysicalDeviceInfo();

    for (uint32_t i = 0; i < physicalDeviceInfo->memoryProperties.memoryTypeCount; i++) {
        if (memoryTypeBits & (1 << i)) {
            const VkMemoryType& type = physicalDeviceInfo->memoryProperties.memoryTypes[i];

            if (type.propertyFlags & requiredFlags) {
                return i;
            }
        }
    }

    return 0;
}

} // API
} // Vulkan
} // Graphics
} // lug
