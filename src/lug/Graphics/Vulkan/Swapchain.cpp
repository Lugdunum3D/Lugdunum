#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/Graphics/Vulkan/RenderPass.hpp>
#include <lug/System/Logger.hpp>

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
    _framebuffers = std::move(swapchain._framebuffers);
    _format = swapchain._format;
    _extent = swapchain._extent;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;
}

Swapchain& Swapchain::operator=(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    _images = std::move(swapchain._images);
    _imagesViews = std::move(swapchain._imagesViews);
    _framebuffers = std::move(swapchain._framebuffers);
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
    // Delete swapchain images and images views
    _framebuffers.clear();
    _imagesViews.clear();
    _images.clear();

    // Delete swapchain
    if (_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(*_device, _swapchain, nullptr);
        _swapchain = VK_NULL_HANDLE;
        _device = nullptr;
    }
}

bool Swapchain::init(CommandBuffer& commandBuffer, RenderPass* renderPass) {
    return initImages(commandBuffer) && initFramebuffers(renderPass);
}

bool Swapchain::initFramebuffers(RenderPass* renderPass) {
    if (!renderPass) {
        LUG_LOG.error("RendererVulkan: initFramebuffers(): renderPass is null");
        return false;
    }

    VkResult result;
    _framebuffers.clear();
    _framebuffers.resize(_imagesViews.size());

    for (size_t i = 0; i < _imagesViews.size(); i++) {
        VkImageView attachments[1]{
            _imagesViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = *renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = _extent.width;
        framebufferInfo.height = _extent.height;
        framebufferInfo.layers = 1;

        VkFramebuffer fb;
        result = vkCreateFramebuffer(*_device, &framebufferInfo, nullptr, &fb);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Failed to create framebuffer: {}", result);
            return false;
        }
        _framebuffers[i] = Framebuffer(fb, _device);
    }
    return true;
}

bool Swapchain::initImages(CommandBuffer& commandBuffer) {
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
        commandBuffer.begin();
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

            _images[i].changeLayout(commandBuffer,
                            0,
                            VK_ACCESS_MEMORY_READ_BIT,
                            VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        }
            commandBuffer.end();
    }

    return true;
}

bool Swapchain::getNextImage(uint32_t *imageIndex, VkSemaphore semaphore) {
    // Get next image
    // TODO: remove UINT64_MAX timeout and ask next image later if VK_NOT_READY is returned
    VkResult result = vkAcquireNextImageKHR(*_device, _swapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, imageIndex);
    if (result != VK_SUCCESS) {
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

std::vector<Image>& Swapchain::getImages() {
    return _images;
}

const std::vector<Framebuffer>& Swapchain::getFramebuffers() const {
    return _framebuffers;
}

const VkSurfaceFormatKHR& Swapchain::getFormat() const {
    return _format;
}

const VkExtent2D& Swapchain::getExtent() const {
    return _extent;
}


} // Vulkan
} // Graphics
} // lug
