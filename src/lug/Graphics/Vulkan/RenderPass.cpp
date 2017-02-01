#include <lug/Graphics/Vulkan/RenderPass.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Framebuffer.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

RenderPass::RenderPass(VkRenderPass renderPass, const Device* device) : _renderPass(renderPass), _device(device) {}

RenderPass::RenderPass(RenderPass&& renderPass) {
    _renderPass = renderPass._renderPass;
    _device = renderPass._device;
    renderPass._renderPass = VK_NULL_HANDLE;
    renderPass._device = nullptr;
}

RenderPass& RenderPass::operator=(RenderPass&& renderPass) {
    destroy();

    _renderPass = renderPass._renderPass;
    _device = renderPass._device;
    renderPass._renderPass = VK_NULL_HANDLE;
    renderPass._device = nullptr;

    return *this;
}

RenderPass::~RenderPass() {
    destroy();
}

void RenderPass::destroy() {
    if (_renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(*_device, _renderPass, nullptr);
        _renderPass = VK_NULL_HANDLE;
    }
}

void RenderPass::begin(const CommandBuffer* commandBuffer,
                        const Framebuffer& framebuffer,
                        const Math::Vec2f& renderExtent,
                        const Math::Vec2f& renderOffset,
                        VkSubpassContents contents) {
    VkClearValue clearColor{};
    clearColor.color = {{0.0f, 0.0f, 0.0f, 1.0f}};

    VkRenderPassBeginInfo beginInfo{
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        beginInfo.pNext = nullptr,
        beginInfo.renderPass = _renderPass,
        beginInfo.framebuffer = framebuffer,
        {}, // beginInfo.renderArea
        beginInfo.clearValueCount = 1,
        beginInfo.pClearValues = &clearColor
    };

    beginInfo.renderArea.offset = {(int32_t)renderOffset.x(), (int32_t)renderOffset.y()};
    beginInfo.renderArea.extent = {(uint32_t)renderExtent.x(), (uint32_t)renderExtent.y()};

    vkCmdBeginRenderPass(*commandBuffer, &beginInfo, contents);
}

void RenderPass::end(const CommandBuffer* commandBuffer) {
    vkCmdEndRenderPass(*commandBuffer);
}

std::unique_ptr<RenderPass> RenderPass::create(const Device* device) {

    VkAttachmentDescription colorAttachment{
        colorAttachment.flags = 0,
        colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM,
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT,
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkAttachmentReference colorAttachmentRef{
        colorAttachmentRef.attachment = 0,
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass{
        subpass.flags = 0,
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        subpass.inputAttachmentCount = 0,
        subpass.pInputAttachments = nullptr,
        subpass.colorAttachmentCount = 1,
        subpass.pColorAttachments = &colorAttachmentRef,
        subpass.pResolveAttachments = nullptr,
        subpass.pDepthStencilAttachment = nullptr,
        subpass.preserveAttachmentCount = 0,
        subpass.pPreserveAttachments = nullptr
    };

    VkRenderPassCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.attachmentCount = 1,
        createInfo.pAttachments = &colorAttachment,
        createInfo.subpassCount = 1,
        createInfo.pSubpasses = &subpass,
        createInfo.dependencyCount = 0,
        createInfo.pDependencies = nullptr
    };

    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkResult result = vkCreateRenderPass(*device, &createInfo, nullptr, &renderPass);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create render pass: {}", result);
        return nullptr;
    }

    return std::unique_ptr<RenderPass>(new RenderPass(renderPass, device));
}

} // Vulkan
} // Graphics
} // lug
