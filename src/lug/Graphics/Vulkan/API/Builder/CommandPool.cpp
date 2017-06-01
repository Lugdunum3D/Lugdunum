#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

CommandPool::CommandPool(const API::Device& device, const API::QueueFamily& queueFamily) : _device{device}, _queueFamily(queueFamily) {}

bool CommandPool::build(API::CommandPool& commandPool, VkResult* returnResult) {
    // Create the commandPool creation information for vkCreateCommandPool
    const VkCommandPoolCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ _flags,
        /* createInfo.queueFamilyIndex */ _queueFamily.getIdx()
    };

    // Create the commandPool
    VkCommandPool vkCommandPool{VK_NULL_HANDLE};
    VkResult result = vkCreateCommandPool(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkCommandPool);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    commandPool = API::CommandPool(vkCommandPool, &_device, nullptr);

    return true;
}

std::unique_ptr<API::CommandPool> CommandPool::build(VkResult* returnResult) {
    std::unique_ptr<API::CommandPool> commandPool = std::make_unique<API::CommandPool>();
    return build(*commandPool, returnResult) ? std::move(commandPool) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
