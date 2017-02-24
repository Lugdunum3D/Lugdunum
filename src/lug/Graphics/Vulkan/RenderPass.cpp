#include <lug/Graphics/Vulkan/RenderPass.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/System/Logger/Logger.hpp>

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
    VkClearValue clearColors[2];
    clearColors[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearColors[1].depthStencil = {
        1.0f, // Depth clear value
        0 // Stecil clear value (We don't care yet)
    };

    VkRenderPassBeginInfo beginInfo{
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        beginInfo.pNext = nullptr,
        beginInfo.renderPass = _renderPass,
        beginInfo.framebuffer = framebuffer,
        {}, // beginInfo.renderArea
        beginInfo.clearValueCount = 2,
        beginInfo.pClearValues = clearColors
    };

    beginInfo.renderArea.offset = {(int32_t)renderOffset.x(), (int32_t)renderOffset.y()};
    beginInfo.renderArea.extent = {(uint32_t)renderExtent.x(), (uint32_t)renderExtent.y()};

    vkCmdBeginRenderPass(*commandBuffer, &beginInfo, contents);
}

void RenderPass::end(const CommandBuffer* commandBuffer) {
    vkCmdEndRenderPass(*commandBuffer);
}

std::unique_ptr<RenderPass> RenderPass::create(const Device* device) {
    LUG_LOG.info("MODIF OK");
VkFormat depthFormat = Image::findSupportedFormat(device,
                                                        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                                        VK_IMAGE_TILING_OPTIMAL,
                                                        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    VkAttachmentDescription attachments [2]{
        // Color attachment
        {
            attachments[0].flags = 0,
            attachments[0].format = VK_FORMAT_R8G8B8A8_UNORM,
            attachments[0].samples = VK_SAMPLE_COUNT_1_BIT,
            attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        },
        // Depth/stencil attachment
        {
            attachments[1].flags = 0,
            attachments[1].format = depthFormat,
            attachments[1].samples = VK_SAMPLE_COUNT_1_BIT,
            attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        }
    };

    VkAttachmentReference colorAttachmentRef{
        colorAttachmentRef.attachment = 0,
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkAttachmentReference depthAttachmentRef{
        depthAttachmentRef.attachment = 1,
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass{
        subpass.flags = 0,
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        subpass.inputAttachmentCount = 0,
        subpass.pInputAttachments = nullptr,
        subpass.colorAttachmentCount = 1,
        subpass.pColorAttachments = &colorAttachmentRef,
        subpass.pResolveAttachments = nullptr,
        subpass.pDepthStencilAttachment = &depthAttachmentRef,
        subpass.preserveAttachmentCount = 0,
        subpass.pPreserveAttachments = nullptr
    };

    VkRenderPassCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.attachmentCount = 2,
        createInfo.pAttachments = attachments,
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
