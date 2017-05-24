#include <lug/Graphics/Vulkan/API/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

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
        vkDestroyRenderPass(static_cast<VkDevice>(*_device), _renderPass, nullptr);
        _renderPass = VK_NULL_HANDLE;
    }
}

void RenderPass::begin(
    const CommandBuffer* commandBuffer,
    const Framebuffer& framebuffer,
    const Math::Vec2f& renderExtent,
    const Math::Vec2f& renderOffset,
    VkSubpassContents contents) {

    VkClearValue clearColors[2];

    clearColors[0].color = {
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    clearColors[1].depthStencil = {
        1.0f, // Depth clear value
        0 // Stecil clear value (We don't care yet)
    };

    VkRenderPassBeginInfo beginInfo{
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        beginInfo.pNext = nullptr,
        beginInfo.renderPass = static_cast<VkRenderPass>(_renderPass),
        beginInfo.framebuffer = static_cast<VkFramebuffer>(framebuffer),
        {}, // beginInfo.renderArea
        beginInfo.clearValueCount = 2,
        beginInfo.pClearValues = clearColors
    };

    beginInfo.renderArea.offset = {(int32_t)renderOffset.x(), (int32_t)renderOffset.y()};
    beginInfo.renderArea.extent = {(uint32_t)renderExtent.x(), (uint32_t)renderExtent.y()};

    vkCmdBeginRenderPass(static_cast<VkCommandBuffer>(*commandBuffer), &beginInfo, contents);
}

void RenderPass::end(const CommandBuffer* commandBuffer) {
    vkCmdEndRenderPass(static_cast<VkCommandBuffer>(*commandBuffer));
}

} // API
} // Vulkan
} // Graphics
} // lug
