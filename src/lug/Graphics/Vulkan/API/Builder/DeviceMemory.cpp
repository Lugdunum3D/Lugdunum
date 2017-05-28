#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>

#include <vector>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

DeviceMemory::DeviceMemory(const API::Device& device) : _device{device} {}

bool DeviceMemory::build(API::DeviceMemory& deviceMemory, VkResult* returnResult) {
    uint32_t memoryTypeIndex = DeviceMemory::findMemoryType(_device, _memoryTypeBits, _memoryFlags);

    // Find the total size and the offset for each elements
    VkDeviceSize size = 0;

    std::vector<VkDeviceSize> offsetBuffers(_buffers.size());
    for (uint32_t i = 0; i < _buffers.size(); ++i) {
        const auto& requirements = _buffers[i]->getRequirements();

        if (size % requirements.alignment) {
            size += requirements.alignment - size % requirements.alignment;
        }

        offsetBuffers[i] = size;
        size += requirements.size;
    }

    std::vector<VkDeviceSize> offsetImages(_images.size());
    for (uint32_t i = 0; i < _images.size(); ++i) {
        const auto& requirements = _images[i]->getRequirements();

        if (size % requirements.alignment) {
            size += requirements.alignment - size % requirements.alignment;
        }

        offsetImages[i] = size;
        size += requirements.size;
    }

    // Create the device memory creation information for vkAllocateMemory
    const VkMemoryAllocateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.allocationSize */ size,
        /* createInfo.memoryTypeIndex */ memoryTypeIndex
    };

    // Create the device memory
    VkDeviceMemory vkDeviceMemory{VK_NULL_HANDLE};
    VkResult result = vkAllocateMemory(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkDeviceMemory);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    deviceMemory = API::DeviceMemory(vkDeviceMemory, &_device, size);

    // Bind all the buffers into the memory
    for (uint32_t i = 0; i < _buffers.size(); ++i) {
        _buffers[i]->bindMemory(deviceMemory, offsetBuffers[i]);
    }

    // Bind all the images into the memory
    for (uint32_t i = 0; i < _images.size(); ++i) {
        _images[i]->bindMemory(deviceMemory, offsetImages[i]);
    }

    return true;
}

std::unique_ptr<API::DeviceMemory> DeviceMemory::build(VkResult* returnResult) {
    std::unique_ptr<API::DeviceMemory> buffer = std::make_unique<API::DeviceMemory>();
    return build(*buffer, returnResult) ? std::move(buffer) : nullptr;
}

bool DeviceMemory::addBuffer(API::Buffer& buffer) {
    _memoryTypeBits &= buffer.getRequirements().memoryTypeBits;

    if (!_memoryTypeBits) {
        return false;
    }

    _buffers.push_back(&buffer);
    return true;
}

bool DeviceMemory::addImage(API::Image& image) {
    _memoryTypeBits &= image.getRequirements().memoryTypeBits;

    if (!_memoryTypeBits) {
        return false;
    }

    _images.push_back(&image);
    return true;
}

uint32_t DeviceMemory::findMemoryType(const API::Device& device, uint32_t memoryTypeBits, VkMemoryPropertyFlags requiredFlags) {
    const PhysicalDeviceInfo* physicalDeviceInfo = device.getPhysicalDeviceInfo();

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

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
