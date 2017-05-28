#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

ImageView::ImageView(const API::Device& device, const API::Image& image) : _device{device}, _image{image} {}

bool ImageView::build(API::ImageView& imageView, VkResult* returnResult) {
    // Create the imageView creation information for vkCreateImageView
    const VkImageViewCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.image */ static_cast<VkImage>(_image),
        /* createInfo.viewType */ _viewType,
        /* createInfo.format */ _format,
        /* createInfo.components */ {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A},
        /* createInfo.subresourceRange */ {
            /* createInfo.subresourceRange.aspectMask */ _aspectFlags,
            /* createInfo.subresourceRange.baseMipLevel */ 0,
            /* createInfo.subresourceRange.levelCount */ 1,
            /* createInfo.subresourceRange.baseArrayLayer */ 0,
            /* createInfo.subresourceRange.layerCount */ 1
        }
    };

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
