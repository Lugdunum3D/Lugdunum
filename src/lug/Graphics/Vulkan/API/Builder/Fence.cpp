#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Fence::Fence(const API::Device& device) : _device{device} {}

bool Fence::build(API::Fence& fence, VkResult* returnResult) {
    // Create the fence creation information for vkCreateFence
    const VkFenceCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ VK_FENCE_CREATE_SIGNALED_BIT
    };

    // Create the fence
    VkFence vkFence{VK_NULL_HANDLE};
    VkResult result = vkCreateFence(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkFence);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    fence = API::Fence(vkFence, &_device);

    return true;
}

std::unique_ptr<API::Fence> Fence::build(VkResult* returnResult) {
    std::unique_ptr<API::Fence> fence = std::make_unique<API::Fence>();
    return build(*fence, returnResult) ? std::move(fence) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
