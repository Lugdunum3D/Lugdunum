#include <lug/Graphics/Vulkan/API/Builder/DescriptorPool.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

DescriptorPool::DescriptorPool(const API::Device& device) : _device{device} {}

bool DescriptorPool::build(API::DescriptorPool& descriptorPool, VkResult* returnResult) {
    // Create the descriptorPool creation information for vkCreateDescriptorPool
    const VkDescriptorPoolCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ _flags,
        /* createInfo.maxSets */ _maxSets,
        /* createInfo.poolSizeCount */ static_cast<uint32_t>(_poolSizes.size()),
        /* createInfo.pPoolSizes */ _poolSizes.data(),
    };

    // Create the descriptorPool
    VkDescriptorPool vkDescriptorPool{VK_NULL_HANDLE};
    VkResult result = vkCreateDescriptorPool(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkDescriptorPool);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    descriptorPool = API::DescriptorPool(vkDescriptorPool, &_device);

    return true;
}

std::unique_ptr<API::DescriptorPool> DescriptorPool::build(VkResult* returnResult) {
    std::unique_ptr<API::DescriptorPool> descriptorPool = std::make_unique<API::DescriptorPool>();
    return build(*descriptorPool, returnResult) ? std::move(descriptorPool) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
