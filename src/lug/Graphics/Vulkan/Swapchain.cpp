#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Swapchain::Swapchain(VkSwapchainKHR swapchain, const Device* device) : _swapchain(swapchain), _device(device) {}

Swapchain::Swapchain(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;
}

Swapchain& Swapchain::operator=(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;

    return *this;
}

Swapchain::~Swapchain() {
    destroy();
}

void Swapchain::destroy() {
    // Delete swapchain images and images views
    for (VkImageView& imageView: _imagesViews) {
        vkDestroyImageView(*_device, imageView, nullptr);
    }
    _imagesViews.clear();
    _images.clear();

    // Delete swapchain
    if (_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(*_device, _swapchain, nullptr);
        _swapchain = VK_NULL_HANDLE;
        _device = nullptr;
    }
}

bool Swapchain::initImages(const VkSurfaceFormatKHR& swapchainFormat) {
    VkResult result;

    // Get swapchain images
    {
        uint32_t imagesCount = 0;
        result = vkGetSwapchainImagesKHR(*_device, _swapchain, &imagesCount, nullptr);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't enumerate swapchain images: {}", result);
            return false;
        }

        _images.resize(imagesCount);
        result = vkGetSwapchainImagesKHR(*_device, _swapchain, &imagesCount, _images.data());
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't enumerate swapchain images: {}", result);
            return false;
        }
    }

    // Create image views
    {
        for (const VkImage& image: _images) {
            // Image subresource range
            VkImageSubresourceRange subresourceRange{
                subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                subresourceRange.baseMipLevel = 0,
                subresourceRange.levelCount = 1,
                subresourceRange.baseArrayLayer = 0,
                subresourceRange.layerCount = 1
            };

            // Image view creation informations
            VkImageViewCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = 0,
                createInfo.image = image,
                createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D,
                createInfo.format = swapchainFormat.format,
                createInfo.components = {
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_G,
                    VK_COMPONENT_SWIZZLE_B,
                    VK_COMPONENT_SWIZZLE_A
                },
                createInfo.subresourceRange = subresourceRange
            };

            VkImageView imageView;
            result = vkCreateImageView(*_device, &createInfo, nullptr, &imageView);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't create swapchain image view: {}", result);
                return false;
            }

            _imagesViews.push_back(imageView);
        }
    }

    return true;
}

} // Vulkan
} // Graphics
} // lug
