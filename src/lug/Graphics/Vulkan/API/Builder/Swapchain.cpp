#define NOMINMAX

#include <lug/Graphics/Vulkan/API/Builder/Swapchain.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Swapchain::Swapchain(const API::Device& device): _device(device) {}

bool Swapchain::build(API::Swapchain& swapchain, VkResult* returnResult) {
    std::vector<uint32_t> queueFamilyIndices(_queueFamilyIndices.begin(), _queueFamilyIndices.end());
    VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // If we have move than one queueFamilyIndices and exclusive was not manually set
    if (queueFamilyIndices.size() > 1 && !_exclusive) {
        sharingMode = VK_SHARING_MODE_CONCURRENT;
    }

    // Create the swapchain creation information for vkCreateSwapchain
    VkSwapchainCreateInfoKHR createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.surface = _surface,
        createInfo.minImageCount = _minImageCount,
        createInfo.imageFormat = _imageFormat,
        createInfo.imageColorSpace = _imageColorSpace,
        {}, // createInfo.imageExtent
        createInfo.imageArrayLayers = 1,
        createInfo.imageUsage = _imageUsage,
        createInfo.imageSharingMode = sharingMode,
        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size()),
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data(),  // Convert the set to raw data,  // Convert the set to raw data
        createInfo.preTransform = _preTransform,
        createInfo.compositeAlpha = _compositeAlpha,
        createInfo.presentMode = _presentMode,
        createInfo.clipped = _clipped,
        createInfo.oldSwapchain = _oldSwapchain
    };

    createInfo.imageExtent.width = _imageExtent.width;
    createInfo.imageExtent.height = _imageExtent.height;


    // Create the swapchain
    VkSwapchainKHR vkSwapchain{VK_NULL_HANDLE};
    VkResult result = vkCreateSwapchainKHR(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkSwapchain);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    swapchain = API::Swapchain(vkSwapchain, &_device, {_imageFormat, _imageColorSpace}, _imageExtent);

    return swapchain.init();
}

std::unique_ptr<API::Swapchain> Swapchain::build(VkResult* returnResult) {
    std::unique_ptr<API::Swapchain> swapchain = std::make_unique<API::Swapchain>();
    return build(*swapchain, returnResult) ? std::move(swapchain) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
