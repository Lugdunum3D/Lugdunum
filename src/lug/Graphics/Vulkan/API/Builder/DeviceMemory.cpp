#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>

#include <vector>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

static VkDeviceSize getAlignedSize(const VkMemoryRequirements& requirements);

DeviceMemory::DeviceMemory(const API::Device& device) : _device{device} {}

bool DeviceMemory::build(API::DeviceMemory& deviceMemory, VkResult* returnResult) {
    uint32_t memoryTypeIndex = API::DeviceMemory::findMemoryType(&_device, _memoryTypeBits, _memoryFlags);

    // Create the device memory creation information for vkAllocateMemory
    VkMemoryAllocateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.allocationSize = _size,
        createInfo.memoryTypeIndex = memoryTypeIndex
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

    deviceMemory = API::DeviceMemory(vkDeviceMemory, &_device);

    // Bind all the buffers into the memory
    VkDeviceSize offset = 0;
    for (API::Buffer* buffer: _buffers) {
        buffer->bindMemory(&deviceMemory, offset);

        auto& requirements = buffer->getRequirements();
        offset += getAlignedSize(requirements);
    }

    // Bind all the images into the memory
    for (API::Image* image: _images) {
        image->bindMemory(&deviceMemory, offset);

        auto& requirements = image->getRequirements();
        offset += getAlignedSize(requirements);
    }

    return true;
}

std::unique_ptr<API::DeviceMemory> DeviceMemory::build(VkResult* returnResult) {
    std::unique_ptr<API::DeviceMemory> buffer = std::make_unique<API::DeviceMemory>();
    return build(*buffer, returnResult) ? std::move(buffer) : nullptr;
}

bool DeviceMemory::addBuffer(API::Buffer& buffer) {
    if (!addRequirement(buffer.getRequirements())) {
        return false;
    }

    _buffers.push_back(&buffer);
    return true;
}

bool DeviceMemory::addImage(API::Image& image) {
    if (!addRequirement(image.getRequirements())) {
        return false;
    }

    _images.push_back(&image);
    return true;
}

bool DeviceMemory::addRequirement(const VkMemoryRequirements& requirements) {
    _memoryTypeBits &= requirements.memoryTypeBits;

    if (!_memoryTypeBits) {
        return false;
    }

    _size += getAlignedSize(requirements);
    return true;
}

VkDeviceSize getAlignedSize(const VkMemoryRequirements& requirements) {
    if (requirements.size % requirements.alignment) {
        return requirements.size + requirements.alignment - requirements.size % requirements.alignment;
    }
    return requirements.size;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
