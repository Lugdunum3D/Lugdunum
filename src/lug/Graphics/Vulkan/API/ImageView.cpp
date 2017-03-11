#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

ImageView::ImageView(VkImageView imageView, const Device* device, const Extent& extent) : _imageView(imageView), _device(device), _extent(extent) {}

ImageView::ImageView(ImageView&& imageView) {
    _imageView = imageView._imageView;
    _device = imageView._device;
    _extent = imageView._extent;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;
    imageView._extent = {0, 0};
}

ImageView& ImageView::operator=(ImageView&& imageView) {
    destroy();

    _imageView = imageView._imageView;
    _device = imageView._device;
    _extent = imageView._extent;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;
    imageView._extent = {0, 0};

    return *this;
}

ImageView::~ImageView() {
    destroy();
}

void ImageView::destroy() {
    if (_imageView != VK_NULL_HANDLE) {
        vkDestroyImageView(static_cast<VkDevice>(*_device), _imageView, nullptr);
        _imageView = VK_NULL_HANDLE;
    }
}

std::unique_ptr<ImageView> ImageView::create(
    const Device* device,
    const Image* image,
    VkFormat format,
    VkImageAspectFlags imageAspect,
    VkImageViewType viewType
) {
    VkImageViewCreateInfo createInfo {
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.image = static_cast<VkImage>(*image),
        createInfo.viewType = viewType,
        createInfo.format = format,
        {}, // createInfo.components
        {}, // createInfo.subresourceRange
    };

    // Image view component mapping
    createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_A;

    // Image subresource range
    createInfo.subresourceRange.aspectMask = imageAspect;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VkImageView imageViewHandle = VK_NULL_HANDLE;
    VkResult result = vkCreateImageView(static_cast<VkDevice>(*device), &createInfo, nullptr, &imageViewHandle);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create image view: {}", result);
        return nullptr;
    }

    Extent extent = {image->getExtent().width, image->getExtent().height};
    return std::unique_ptr<ImageView>(new ImageView(imageViewHandle, device, extent));
}

} // API
} // Vulkan
} // Graphics
} // lug
