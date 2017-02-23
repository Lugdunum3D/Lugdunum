#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/Graphics/Vulkan/RenderPass.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Swapchain::Swapchain(VkSwapchainKHR swapchain, const Device* device, const VkSurfaceFormatKHR& swapchainFormat, const VkExtent2D& extent) :
                    _swapchain(swapchain), _device(device), _format(swapchainFormat), _extent(extent) {}

Swapchain::Swapchain(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    _images = std::move(swapchain._images);
    _imagesViews = std::move(swapchain._imagesViews);
    _format = swapchain._format;
    _extent = swapchain._extent;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;
}

Swapchain& Swapchain::operator=(Swapchain&& swapchain) {
    destroy();

    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    _images = std::move(swapchain._images);
    _imagesViews = std::move(swapchain._imagesViews);
    _format = swapchain._format;
    _extent = swapchain._extent;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;

    return *this;
}

Swapchain::~Swapchain() {
    destroy();
}

void Swapchain::destroy() {
    _outOfDate = false;

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

bool Swapchain::init() {
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
            _images[i] = Image(images[i], _device, {_extent.width, _extent.height}, true);
        }
    }

    // Create image views
    {
        _imagesViews.resize(_images.size());
        for (uint8_t i = 0; i < _images.size(); ++i) {
            std::unique_ptr<ImageView> imageView = ImageView::create(_device, &_images[i], _format.format);
            if (!imageView) {
                LUG_LOG.error("RendererVulkan: Can't create swapchain image view");
                return false;
            }
            _imagesViews[i] = std::move(imageView);
        }
    }

    return true;
}

bool Swapchain::getNextImage(uint32_t *imageIndex, VkSemaphore semaphore) {
    // Get next image
    // TODO: remove UINT64_MAX timeout and ask next image later if VK_NOT_READY is returned
    VkResult result = vkAcquireNextImageKHR(*_device, _swapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        _outOfDate = true;
        return false;
    } else if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: getNextImage(): Can't acquire swapchain next image: {}", result);
        return false;
    }

    return true;
}

bool Swapchain::present(const Queue* presentQueue, uint32_t imageIndex, VkSemaphore semaphore) {
    // Present image
    VkPresentInfoKHR presentInfo{
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        presentInfo.pNext = nullptr,
        presentInfo.waitSemaphoreCount = semaphore != VK_NULL_HANDLE ? 1 : 0,
        presentInfo.pWaitSemaphores = semaphore != VK_NULL_HANDLE ? &semaphore : nullptr,
        presentInfo.swapchainCount = 1,
        presentInfo.pSwapchains = &_swapchain,
        presentInfo.pImageIndices = &imageIndex,
        presentInfo.pResults = nullptr
    };

    VkResult result = vkQueuePresentKHR(*presentQueue, &presentInfo);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: present(): Can't acquire swapchain next image: {}", result);
        return false;
    }
    return true;
}

} // Vulkan
} // Graphics
} // lug
