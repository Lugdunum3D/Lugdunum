#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Swapchain::Swapchain(VkSwapchainKHR swapchain, const Device* device, const VkSurfaceFormatKHR& swapchainFormat) :
                    _swapchain(swapchain), _device(device), _format(swapchainFormat) {}

Swapchain::Swapchain(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    _format = swapchain._format;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;
}

Swapchain& Swapchain::operator=(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    _format = swapchain._format;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;

    return *this;
}

Swapchain::~Swapchain() {
    destroy();
}

void Swapchain::destroy() {
    // Delete swapchain images and images views
    _imagesViews.clear();
    _images.clear();

    // Delete swapchain
    if (_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(*_device, _swapchain, nullptr);
        _swapchain = VK_NULL_HANDLE;
        _device = nullptr;
    }
}

bool Swapchain::initImages() {
    VkResult result;

    // Get swapchain images
    {
        uint32_t imagesCount = 0;
        std::vector<VkImage> images;

        result = vkGetSwapchainImagesKHR(*_device, _swapchain, &imagesCount, nullptr);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't enumerate swapchain images: {}", result);
            return false;
        }

        images.resize(imagesCount);
        result = vkGetSwapchainImagesKHR(*_device, _swapchain, &imagesCount, images.data());
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't enumerate swapchain images: {}", result);
            return false;
        }

        // Copy VkImage vector to Image vector
        _images.resize(imagesCount);
        for (uint8_t i = 0; i < images.size(); ++i) {
            _images[i] = Image(images[i], _device, true);
        }
    }

    // Create image views
    {
        _imagesViews.resize(_images.size());
        for (uint8_t i = 0; i < _images.size(); ++i) {
            // Image view creation informations
            VkImageViewCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = 0,
                createInfo.image = _images[i],
                createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D,
                createInfo.format = _format.format,
                {}, // createInfo.components
                {}, // createInfo.subresourceRange
            };

            // Image view component mapping
            createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_A;

            // Image subresource range
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            VkImageView imageView = VK_NULL_HANDLE;
            result = vkCreateImageView(*_device, &createInfo, nullptr, &imageView);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't create swapchain image view: {}", result);
                return false;
            }

            _imagesViews[i] = ImageView(imageView, _device);
        }
    }

    return true;
}

} // Vulkan
} // Graphics
} // lug
