#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.hpp>

#include <algorithm>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/RenderPass.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Framebuffer::Framebuffer(const API::Device& device) : _device{device} {}

bool Framebuffer::build(API::Framebuffer& framebuffer, VkResult* returnResult) {
    // Build a vector of VkImageView from the API::ImageView
    std::vector<VkImageView> vkImageViews(_attachments.size());
    std::transform(
        begin(_attachments), end(_attachments), begin(vkImageViews),
        [](const API::ImageView* imageView){ return static_cast<VkImageView>(*imageView); }
    );

    // Create the framebuffer creation information for vkCreateFramebuffer
    const VkFramebufferCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.renderPass */ static_cast<VkRenderPass>(*_renderPass),
        /* createInfo.attachmentCount */ static_cast<uint32_t>(vkImageViews.size()),
        /* createInfo.pAttachments */ vkImageViews.data(),
        /* createInfo.width */ _width,
        /* createInfo.height */ _height,
        /* createInfo.layers */ _layers
    };

    // Create the framebuffer
    VkFramebuffer vkFramebuffer{VK_NULL_HANDLE};
    VkResult result = vkCreateFramebuffer(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkFramebuffer);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    framebuffer = API::Framebuffer(vkFramebuffer, &_device, {_width, _height});

    return true;
}

std::unique_ptr<API::Framebuffer> Framebuffer::build(VkResult* returnResult) {
    std::unique_ptr<API::Framebuffer> framebuffer = std::make_unique<API::Framebuffer>();
    return build(*framebuffer, returnResult) ? std::move(framebuffer) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
