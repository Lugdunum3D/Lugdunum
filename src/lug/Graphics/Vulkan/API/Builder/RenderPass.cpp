#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>

#include <algorithm>
#include <vector>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

RenderPass::RenderPass(const API::Device& device) : _device{device} {}

RenderPass::AttachmentIndex RenderPass::addAttachment(const VkAttachmentDescription& attachmentDescription) {
    _attachments.push_back(attachmentDescription);
    return &_attachments.back();
}

RenderPass::SubpassIndex RenderPass::addSubpass(const RenderPass::SubpassDescription& subpassDescription) {
    const auto findIndexAttachment = [this](RenderPass::AttachmentIndex idx) {
        uint32_t i = 0;

        for (const auto& attachment : _attachments) {
            if (&attachment == idx) {
                break;
            }

            ++i;
        }

        return i;
    };

    const auto transformAttachmentReference = [&findIndexAttachment](const RenderPass::AttachmentReference& reference){
        return VkAttachmentReference{
            findIndexAttachment(reference.attachment),
            reference.layout
        };
    };

    SubpassResources resources{};

    resources.inputAttachments.resize(subpassDescription.inputAttachments.size());
    std::transform(
        std::begin(subpassDescription.inputAttachments), std::end(subpassDescription.inputAttachments), std::begin(resources.inputAttachments),
        transformAttachmentReference
    );

    resources.colorAttachments.resize(subpassDescription.colorAttachments.size());
    std::transform(
        std::begin(subpassDescription.colorAttachments), std::end(subpassDescription.colorAttachments), std::begin(resources.colorAttachments),
        transformAttachmentReference
    );

    resources.resolveAttachments.resize(subpassDescription.resolveAttachments.size());
    std::transform(
        std::begin(subpassDescription.resolveAttachments), std::end(subpassDescription.resolveAttachments), std::begin(resources.resolveAttachments),
        transformAttachmentReference
    );

    resources.depthStencilAttachment = transformAttachmentReference(subpassDescription.depthStencilAttachment);

    resources.preserveAttachments.resize(subpassDescription.preserveAttachments.size());
    std::transform(
        std::begin(subpassDescription.preserveAttachments), std::end(subpassDescription.preserveAttachments), std::begin(resources.preserveAttachments),
        findIndexAttachment
    );

    _subpassesResources.push_back(std::move(resources));

    const VkSubpassDescription vkSubpassDescription{
        /* vkSubpassDescription.flags */ 0,
        /* vkSubpassDescription.pipelineBindPoint */ subpassDescription.pipelineBindPoint,
        /* vkSubpassDescription.inputAttachmentCount */ static_cast<uint32_t>(_subpassesResources.back().inputAttachments.size()),
        /* vkSubpassDescription.pInputAttachments */ _subpassesResources.back().inputAttachments.data(),
        /* vkSubpassDescription.colorAttachmentCount */ static_cast<uint32_t>(_subpassesResources.back().colorAttachments.size()),
        /* vkSubpassDescription.pColorAttachments */ _subpassesResources.back().colorAttachments.data(),
        /* vkSubpassDescription.pResolveAttachments */ _subpassesResources.back().resolveAttachments.data(),
        /* vkSubpassDescription.pDepthStencilAttachment */ &_subpassesResources.back().depthStencilAttachment,
        /* vkSubpassDescription.preserveAttachmentCount */ static_cast<uint32_t>(_subpassesResources.back().preserveAttachments.size()),
        /* vkSubpassDescription.pPreserveAttachments */ _subpassesResources.back().preserveAttachments.data(),
    };

    _subpasses.push_back(std::move(vkSubpassDescription));
    return &_subpasses.back();
}

void RenderPass::addSubpassDependency(const RenderPass::SubpassDependency& subpassDependency) {
    const auto findIndexSubpass = [this](RenderPass::SubpassIndex idx) {
        uint32_t i = 0;

        for (const auto& subpass : _subpasses) {
            if (&subpass == idx) {
                break;
            }

            ++i;
        }

        return i;
    };

    const VkSubpassDependency vkSubpassDependency{
        /* vkSubpassDependency.srcSubpass */ findIndexSubpass(subpassDependency.srcSubpass),
        /* vkSubpassDependency.dstSubpass */ findIndexSubpass(subpassDependency.dstSubpass),
        /* vkSubpassDependency.srcStageMask */ subpassDependency.srcStageMask,
        /* vkSubpassDependency.dstStageMask */ subpassDependency.dstStageMask,
        /* vkSubpassDependency.srcAccessMask */ subpassDependency.srcAccessMask,
        /* vkSubpassDependency.dstAccessMask */ subpassDependency.dstAccessMask,
        /* vkSubpassDependency.dependencyFlags */ subpassDependency.dependencyFlags
    };

    _dependencies.push_back(std::move(vkSubpassDependency));
}

bool RenderPass::build(API::RenderPass& renderPass, VkResult* returnResult) {
    const std::vector<VkAttachmentDescription> attachments{std::cbegin(_attachments), std::cend(_attachments)};
    const std::vector<VkSubpassDescription> subpasses{std::cbegin(_subpasses), std::cend(_subpasses)};
    const std::vector<VkSubpassDependency> dependencies{std::cbegin(_dependencies), std::cend(_dependencies)};

    const VkRenderPassCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.attachmentCount */ static_cast<uint32_t>(attachments.size()),
        /* createInfo.pAttachments */ attachments.data(),
        /* createInfo.subpassCount */ static_cast<uint32_t>(subpasses.size()),
        /* createInfo.pSubpasses */ subpasses.data(),
        /* createInfo.dependencyCount */ static_cast<uint32_t>(dependencies.size()),
        /* createInfo.pDependencies */ dependencies.data()
    };

    // Create the render pass
    VkRenderPass vkRenderPass{VK_NULL_HANDLE};
    VkResult result = vkCreateRenderPass(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkRenderPass);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    renderPass = API::RenderPass(vkRenderPass, &_device);

    return true;
}

std::unique_ptr<API::RenderPass> RenderPass::build(VkResult* returnResult) {
    std::unique_ptr<API::RenderPass> renderPass = std::make_unique<API::RenderPass>();
    return build(*renderPass, returnResult) ? std::move(renderPass) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
