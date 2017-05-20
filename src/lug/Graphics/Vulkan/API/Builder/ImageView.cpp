#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>

#include <vector>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

ImageView::ImageView(const API::Device& device, const API::Image& image) : _device{device}, _image{image} {}

bool ImageView::build(API::ImageView& imageView, VkResult* returnResult) {

    // Create the imageView creation information for vkCreateImageView
    VkImageViewCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.image = static_cast<VkImage>(_image),
        createInfo.viewType = _viewType,
        createInfo.format = _format,
        {}, // createInfo.components
        {}, // createInfo.subresourceRange
    };

    // Image view component mapping
    createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_A;

    // Image subresource range
    createInfo.subresourceRange.aspectMask = _aspectFlags;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    // Create the imageView
    VkImageView vkImageView{VK_NULL_HANDLE};
    VkResult result = vkCreateImageView(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkImageView);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    imageView = API::ImageView(vkImageView, &_device, &_image);

    return true;
}

std::unique_ptr<API::ImageView> ImageView::build(VkResult* returnResult) {
    std::unique_ptr<API::ImageView> imageView = std::make_unique<API::ImageView>();
    return build(*imageView, returnResult) ? std::move(imageView) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
