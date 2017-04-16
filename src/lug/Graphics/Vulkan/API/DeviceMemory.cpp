#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

DeviceMemory::DeviceMemory(VkDeviceMemory buffer, const Device* device) : _deviceMemory(buffer), _device(device) {}

DeviceMemory::DeviceMemory(DeviceMemory&& buffer) {
    _deviceMemory = buffer._deviceMemory;
    _device = buffer._device;
    buffer._deviceMemory = VK_NULL_HANDLE;
    buffer._device = nullptr;
}

DeviceMemory& DeviceMemory::operator=(DeviceMemory&& buffer) {
    destroy();

    _deviceMemory = buffer._deviceMemory;
    _device = buffer._device;
    buffer._deviceMemory = VK_NULL_HANDLE;
    buffer._device = nullptr;

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

std::unique_ptr<DeviceMemory> DeviceMemory::allocate(const Device* device, VkDeviceSize size, uint32_t memoryTypeIndex) {
    VkMemoryAllocateInfo allocateInfo{
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        allocateInfo.pNext = nullptr,
        allocateInfo.allocationSize = size,
        allocateInfo.memoryTypeIndex = memoryTypeIndex
    };

    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkResult result = vkAllocateMemory(static_cast<VkDevice>(*device), &allocateInfo, nullptr, &memory);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't allocate device memory: {}", result);
        return nullptr;
    }

    return std::unique_ptr<DeviceMemory>(new DeviceMemory(memory, device));
}

uint32_t DeviceMemory::findMemoryType(const Device* device, const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags requiredFlags) {
    const PhysicalDeviceInfo* physicalDeviceInfo = device->getPhysicalDeviceInfo();

    for (uint32_t i = 0; i < physicalDeviceInfo->memoryProperties.memoryTypeCount; i++) {
        if (memoryRequirements.memoryTypeBits & (1 << i)) {
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
