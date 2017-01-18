#include <lug/Graphics/Vulkan/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Image::Image(VkImage image, const Device* device, bool swapchainImage, VkImageAspectFlags aspect) :
            _image(image), _device(device), _swapchainImage(swapchainImage), _aspect(aspect) {}

Image::Image(Image&& image) {
    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _aspect = image._aspect;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
}

Image& Image::operator=(Image&& image) {
    destroy();

    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _aspect = image._aspect;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;

    return *this;
}

Image::~Image() {
    // Swapchain images are automatically destroyed when destroying swapchain
    if (!_swapchainImage) {
        destroy();
    }
}

void Image::changeLayout(CommandBuffer& commandBuffer,
                VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                VkImageLayout oldLayout, VkImageLayout newLayout,
                VkPipelineStageFlags srcStageMask,
                VkPipelineStageFlags dstStageMask,
                uint32_t srcQueueFamilyIndex,
                uint32_t dstQueueFamilyIndex) {

    VkImageMemoryBarrier imageBarrier{
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        imageBarrier.pNext = nullptr,
        imageBarrier.srcAccessMask = srcAccessMask,
        imageBarrier.dstAccessMask = dstAccessMask,
        imageBarrier.oldLayout = oldLayout,
        imageBarrier.newLayout = newLayout,
        imageBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex,
        imageBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex,
        imageBarrier.image = _image,
        {} // imageBarrier.subresourceRange
    };

    imageBarrier.subresourceRange.aspectMask = _aspect;
    imageBarrier.subresourceRange.baseMipLevel = 0;
    imageBarrier.subresourceRange.levelCount = 1;
    imageBarrier.subresourceRange.baseArrayLayer = 0;
    imageBarrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(commandBuffer, // commandBuffer
                        srcStageMask, // srcStageMask
                        dstStageMask, // dstStageMask
                        VK_DEPENDENCY_BY_REGION_BIT, // dependencyFlags
                        0, // memoryBarrierCount
                        nullptr, // pMemoryBarriers
                        0, // bufferMemoryBarrierCount
                        nullptr, // pBufferMemoryBarriers
                        1, // imageMemoryBarrierCount
                        &imageBarrier); // pImageMemoryBarriers
}

void Image::destroy() {
    if (_image != VK_NULL_HANDLE) {
        vkDestroyImage(*_device, _image, nullptr);
        _image = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphics
} // lug
