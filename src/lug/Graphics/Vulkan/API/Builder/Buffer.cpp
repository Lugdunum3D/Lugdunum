#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>

#include <vector>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Buffer::Buffer(const API::Device& device) : _device{device} {}

bool Buffer::build(API::Buffer& buffer, VkResult* returnResult) {

    VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // If we have move than one queueFamilyIndices and exclusive was not manually set
    if (_queueFamilyIndices.size() > 1 && !_exclusive) {
        sharingMode = VK_SHARING_MODE_CONCURRENT;
    }

    // Create the buffer creation information for vkCreateBuffer
    VkBufferCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = _flags,
        createInfo.size = _size,
        createInfo.usage = _usage,
        createInfo.sharingMode = sharingMode,
        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(_queueFamilyIndices.size()),
        createInfo.pQueueFamilyIndices = std::vector<uint32_t>(_queueFamilyIndices.begin(), _queueFamilyIndices.end()).data(),  // Convert the set to raw data
    };

    // Create the buffer
    VkBuffer vkBuffer{VK_NULL_HANDLE};
    VkResult result = vkCreateBuffer(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkBuffer);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    buffer = API::Buffer(vkBuffer, &_device);

    return true;
}

std::unique_ptr<API::Buffer> Buffer::build(VkResult* returnResult) {
    std::unique_ptr<API::Buffer> buffer = std::make_unique<API::Buffer>();
    return build(*buffer, returnResult) ? std::move(buffer) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
