#include <lug/Graphics/Vulkan/API/Builder/Swapchain.hpp>

#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Swapchain::Swapchain(const API::Device& device): _device(device) {}

bool Swapchain::setFromPreferences() {
    const PhysicalDeviceInfo* info = _device.getPhysicalDeviceInfo();

    // Set the present mode (_presentMode)
    {
        bool presentModeFound = false;
        for (auto presentMode : _preferences->presentModes) {
            if (std::find(info->swapchain.presentModes.begin(), info->swapchain.presentModes.end(), presentMode) != info->swapchain.presentModes.end()) {
                LUG_LOG.info("Swapchain::build: Use present mode {}", API::RTTI::toStr(presentMode));

                _presentMode = presentMode;
                presentModeFound = true;
                break;
            }
        }

        if (!presentModeFound) {
            LUG_LOG.error("Swapchain::build: Missing present mode supported by Lugdunum");
            return false;
        }
    }

    // Set the format (_imageFormat)
    {
        if (_imageColorSpace == VK_COLOR_SPACE_MAX_ENUM_KHR) {
            LUG_LOG.error("Swapchain::build: Missing imageColorSpace parameter. Call Builder::Swapchain::setImageColorSpace()");
            return false;
        }

        bool formatFound = false;
        auto& localColorSpace = _imageColorSpace; // Define a local variable for lamda capture below
        for (auto imageFormat : _preferences->formats) {
            if (std::find_if(info->swapchain.formats.begin(), info->swapchain.formats.end(), [&localColorSpace, &imageFormat](const VkSurfaceFormatKHR& lhs) {
                return lhs.colorSpace == localColorSpace && imageFormat == lhs.format;
            }) != info->swapchain.formats.end()) {
                LUG_LOG.info("Swapchain::build: Use format {}", API::RTTI::toStr(imageFormat));

                _imageFormat = imageFormat;
                formatFound = true;
                break;
            }
        }

        if (!formatFound) {
            LUG_LOG.error("Swapchain::build: Missing swapchain format supported by Lugdunum");
            return false;
        }
    }

    // Set the composite alpha (_compositeAlpha)
    {
        bool compositeAlphaFound = false;
        for (auto compositeAlpha : _preferences->compositeAlphas) {
            if (info->swapchain.capabilities.supportedCompositeAlpha & compositeAlpha) {
                LUG_LOG.info("Swapchain::build: Use composite alpha {}", API::RTTI::toStr(compositeAlpha));

                _compositeAlpha = compositeAlpha;
                compositeAlphaFound = true;
                break;
            }
        }

        if (!compositeAlphaFound) {
            LUG_LOG.error("Swapchain::build: Missing composite alpha supported by Lugdunum");
            return false;
        }
    }

    return true;
}

bool Swapchain::build(API::Swapchain& swapchain, VkResult* returnResult) {
    const PhysicalDeviceInfo* info = _device.getPhysicalDeviceInfo();

    std::vector<uint32_t> queueFamilyIndices(_queueFamilyIndices.begin(), _queueFamilyIndices.end());
    VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // If we have move than one queueFamilyIndices and exclusive was not manually set
    if (queueFamilyIndices.size() > 1 && !_exclusive) {
        sharingMode = VK_SHARING_MODE_CONCURRENT;
    }

    // Check the image counts
    if (info->swapchain.capabilities.maxImageCount > 0 && info->swapchain.capabilities.maxImageCount < _minImageCount) {
        LUG_LOG.error("RendererWindow: Not enough images ({} required), found {}", _minImageCount, info->swapchain.capabilities.maxImageCount);
        return false;
    }

    if (_preferences != nullptr && !setFromPreferences()) {
        return false;
    }

    // Create the swapchain creation information for vkCreateSwapchain
    const VkSwapchainCreateInfoKHR createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.surface */ _surface,
        /* createInfo.minImageCount */ _minImageCount,
        /* createInfo.imageFormat */ _imageFormat,
        /* createInfo.imageColorSpace */ _imageColorSpace,
        /* createInfo.imageExtent */ {_imageExtent.width, _imageExtent.height},
        /* createInfo.imageArrayLayers */ 1,
        /* createInfo.imageUsage */ _imageUsage,
        /* createInfo.imageSharingMode */ sharingMode,
        /* createInfo.queueFamilyIndexCount */ static_cast<uint32_t>(queueFamilyIndices.size()),
        /* createInfo.pQueueFamilyIndices */ queueFamilyIndices.data(),  // Convert the set to raw data,  // Convert the set to raw data
        /* createInfo.preTransform */ _preTransform,
        /* createInfo.compositeAlpha */ _compositeAlpha,
        /* createInfo.presentMode */ _presentMode,
        /* createInfo.clipped */ _clipped,
        /* createInfo.oldSwapchain */ _oldSwapchain
    };

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
