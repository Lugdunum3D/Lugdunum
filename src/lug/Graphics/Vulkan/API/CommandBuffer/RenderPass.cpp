#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>

#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/RenderPass.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

void CommandBuffer::beginRenderPass(
    const API::RenderPass& renderPass,
    const CommandBuffer::CmdBeginRenderPass& parameters,
    VkSubpassContents contents
) const {
    const VkRenderPassBeginInfo beginInfo{
        /* beginInfo.sType */ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        /* beginInfo.pNext */ nullptr,
        /* beginInfo.renderPass */ static_cast<VkRenderPass>(renderPass),
        /* beginInfo.framebuffer */ static_cast<VkFramebuffer>(parameters.framebuffer),
        /* beginInfo.renderArea */ parameters.renderArea,
        /* beginInfo.clearValueCount */ static_cast<uint32_t>(parameters.clearValues.size()),
        /* beginInfo.pClearValues */ parameters.clearValues.data()
    };

    vkCmdBeginRenderPass(static_cast<VkCommandBuffer>(_commandBuffer), &beginInfo, contents);
}

void CommandBuffer::endRenderPass() const {
    vkCmdEndRenderPass(static_cast<VkCommandBuffer>(_commandBuffer));
}

void CommandBuffer::draw(const CmdDraw& params) const {
    vkCmdDraw(
        static_cast<VkCommandBuffer>(_commandBuffer),
        params.vertexCount,
        params.instanceCount,
        params.firstVertex,
        params.firstInstance
    );
}

void CommandBuffer::drawIndexed(const CmdDrawIndexed& params) const {
    vkCmdDrawIndexed(
        static_cast<VkCommandBuffer>(_commandBuffer),
        params.indexCount,
        params.instanceCount,
        params.firstIndex,
        params.vertexOffset,
        params.firstInstance
    );
}

} // API
} // Vulkan
} // Graphics
} // lug
