#include <lug/Graphics/Vulkan/API/Builder/Semaphore.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Semaphore::Semaphore(const API::Device& device) : _device{device} {}

bool Semaphore::build(API::Semaphore& semaphore, VkResult* returnResult) {
    // Create the semaphore creation information for vkCreateSemaphore
    const VkSemaphoreCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0
    };

    // Create the semaphore
    VkSemaphore vkSemaphore{VK_NULL_HANDLE};
    VkResult result = vkCreateSemaphore(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkSemaphore);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    semaphore = API::Semaphore(vkSemaphore, &_device);

    return true;
}

std::unique_ptr<API::Semaphore> Semaphore::build(VkResult* returnResult) {
    std::unique_ptr<API::Semaphore> semaphore = std::make_unique<API::Semaphore>();
    return build(*semaphore, returnResult) ? std::move(semaphore) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
