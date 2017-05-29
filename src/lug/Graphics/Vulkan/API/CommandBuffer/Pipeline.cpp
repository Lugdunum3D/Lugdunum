#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>

#include <algorithm>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

static VkAccessFlags getImageSrcAccessMask(VkImageLayout oldLayout) {
    switch (oldLayout) {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            return 0;
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            return VK_ACCESS_HOST_WRITE_BIT;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            return VK_ACCESS_TRANSFER_READ_BIT;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            return VK_ACCESS_TRANSFER_WRITE_BIT;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            return VK_ACCESS_SHADER_READ_BIT;
        default:
            // Not handled
            return 0;
    }
}

static VkAccessFlags getImageDstAccessMask(VkImageLayout newLayout) {
    switch (newLayout)
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            return VK_ACCESS_TRANSFER_WRITE_BIT;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            return VK_ACCESS_TRANSFER_READ_BIT;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            return VK_ACCESS_SHADER_READ_BIT;
        default:
            // Not handled
            return 0;
    }
}

static void setupMemoryBarriers(
    std::vector<VkMemoryBarrier>& vkMemoryBarriers,
    const std::vector<CommandBuffer::CmdPipelineBarrier::MemoryBarrier>& memoryBarriers
) {
    vkMemoryBarriers.resize(memoryBarriers.size());
    for (uint8_t i = 0; i < memoryBarriers.size(); ++i){
        vkMemoryBarriers[i].sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        vkMemoryBarriers[i].pNext = nullptr;
        vkMemoryBarriers[i].srcAccessMask = memoryBarriers[i].srcAccessMask;
        vkMemoryBarriers[i].dstAccessMask = memoryBarriers[i].dstAccessMask;
    }
}

static void setupBufferBarriers(
    std::vector<VkBufferMemoryBarrier>& vkBufferMemoryBarriers,
    const std::vector<CommandBuffer::CmdPipelineBarrier::BufferMemoryBarrier>& bufferMemoryBarriers
) {
    vkBufferMemoryBarriers.resize(bufferMemoryBarriers.size());
    for (uint8_t i = 0; i < bufferMemoryBarriers.size(); ++i){
        vkBufferMemoryBarriers[i].sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        vkBufferMemoryBarriers[i].pNext = nullptr;
        vkBufferMemoryBarriers[i].srcAccessMask = bufferMemoryBarriers[i].srcAccessMask;
        vkBufferMemoryBarriers[i].dstAccessMask = bufferMemoryBarriers[i].dstAccessMask;
        vkBufferMemoryBarriers[i].srcQueueFamilyIndex = bufferMemoryBarriers[i].srcQueueFamilyIndex;
        vkBufferMemoryBarriers[i].dstQueueFamilyIndex = bufferMemoryBarriers[i].dstQueueFamilyIndex;
        vkBufferMemoryBarriers[i].buffer = static_cast<VkBuffer>(*bufferMemoryBarriers[i].buffer);
        vkBufferMemoryBarriers[i].offset = bufferMemoryBarriers[i].offset;
        vkBufferMemoryBarriers[i].size = bufferMemoryBarriers[i].size;
    }
}

static void setupImageBarriers(
    std::vector<VkImageMemoryBarrier>& vkImageMemoryBarriers,
    const std::vector<CommandBuffer::CmdPipelineBarrier::ImageMemoryBarrier>& imageMemoryBarriers
) {
    vkImageMemoryBarriers.resize(imageMemoryBarriers.size());
    for (uint8_t i = 0; i < imageMemoryBarriers.size(); ++i){
        vkImageMemoryBarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        vkImageMemoryBarriers[i].pNext = nullptr;
        vkImageMemoryBarriers[i].srcAccessMask = imageMemoryBarriers[i].srcAccessMask;
        vkImageMemoryBarriers[i].dstAccessMask = imageMemoryBarriers[i].dstAccessMask;
        vkImageMemoryBarriers[i].oldLayout = imageMemoryBarriers[i].oldLayout;
        vkImageMemoryBarriers[i].newLayout = imageMemoryBarriers[i].newLayout;
        vkImageMemoryBarriers[i].srcQueueFamilyIndex = imageMemoryBarriers[i].srcQueueFamilyIndex;
        vkImageMemoryBarriers[i].dstQueueFamilyIndex = imageMemoryBarriers[i].dstQueueFamilyIndex;
        vkImageMemoryBarriers[i].image = static_cast<VkImage>(*imageMemoryBarriers[i].image);
        vkImageMemoryBarriers[i].subresourceRange.aspectMask = imageMemoryBarriers[i].subresourceRange.aspectMask;
        vkImageMemoryBarriers[i].subresourceRange.baseMipLevel = imageMemoryBarriers[i].subresourceRange.baseMipLevel;
        vkImageMemoryBarriers[i].subresourceRange.levelCount = imageMemoryBarriers[i].subresourceRange.levelCount;
        vkImageMemoryBarriers[i].subresourceRange.baseArrayLayer = imageMemoryBarriers[i].subresourceRange.baseArrayLayer;
        vkImageMemoryBarriers[i].subresourceRange.layerCount = imageMemoryBarriers[i].subresourceRange.layerCount;

        if (vkImageMemoryBarriers[i].srcAccessMask == 0xFFFFFFFF) {
            vkImageMemoryBarriers[i].srcAccessMask = getImageSrcAccessMask(vkImageMemoryBarriers[i].oldLayout);
        }
        if (vkImageMemoryBarriers[i].dstAccessMask == 0xFFFFFFFF) {
            vkImageMemoryBarriers[i].dstAccessMask = getImageDstAccessMask(vkImageMemoryBarriers[i].newLayout);
        }

        if (vkImageMemoryBarriers[i].srcAccessMask == 0 &&
            vkImageMemoryBarriers[i].newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            vkImageMemoryBarriers[i].srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        }
    }
}

void CommandBuffer::pipelineBarrier(
    const CmdPipelineBarrier& parameters,
    VkDependencyFlags dependencyFlags,
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask
) const {

    std::vector<VkMemoryBarrier> vkMemoryBarriers;
    std::vector<VkBufferMemoryBarrier> vkBufferMemoryBarriers;
    std::vector<VkImageMemoryBarrier> vkImageMemoryBarriers;

    setupMemoryBarriers(vkMemoryBarriers, parameters.memoryBarriers);
    setupBufferBarriers(vkBufferMemoryBarriers, parameters.bufferMemoryBarriers);
    setupImageBarriers(vkImageMemoryBarriers, parameters.imageMemoryBarriers);

    vkCmdPipelineBarrier(
        static_cast<VkCommandBuffer>(_commandBuffer),
        srcStageMask,
        dstStageMask,
        dependencyFlags,
        static_cast<uint32_t>(vkMemoryBarriers.size()),
        vkMemoryBarriers.data(),
        static_cast<uint32_t>(vkBufferMemoryBarriers.size()),
        vkBufferMemoryBarriers.data(),
        static_cast<uint32_t>(vkImageMemoryBarriers.size()),
        vkImageMemoryBarriers.data()
    );
}

void CommandBuffer::bindPipeline(const API::GraphicsPipeline& pipeline, VkPipelineBindPoint pipelineBindPoint) const {
    vkCmdBindPipeline(
        static_cast<VkCommandBuffer>(_commandBuffer),
        pipelineBindPoint,
        static_cast<VkPipeline>(pipeline)
    );
}

void CommandBuffer::bindVertexBuffers(
    const std::vector<const API::Buffer*>& buffers,
    const std::vector<VkDeviceSize>& offsets,
    uint32_t firstBinding
) const {

    // Build a vector of VkBuffer from the API::Buffer
    std::vector<VkBuffer> vkBuffers(buffers.size());
    std::transform(
        std::begin(buffers), std::end(buffers), std::begin(vkBuffers),
        [](const API::Buffer* buffer){ return static_cast<VkBuffer>(*buffer); }
    );

    vkCmdBindVertexBuffers(
        static_cast<VkCommandBuffer>(_commandBuffer),
        firstBinding,
        static_cast<uint32_t>(vkBuffers.size()),
        vkBuffers.data(),
        offsets.data()
    );
}

void CommandBuffer::bindIndexBuffer(const API::Buffer& buffer, VkIndexType indexType, VkDeviceSize offset) const {
    vkCmdBindIndexBuffer(
        static_cast<VkCommandBuffer>(_commandBuffer),
        static_cast<VkBuffer>(buffer),
        offset,
        indexType
    );
}

void CommandBuffer::setViewport(const std::vector<VkViewport>& viewports, uint32_t firstViewport) const {
    vkCmdSetViewport(
        static_cast<VkCommandBuffer>(_commandBuffer),
        firstViewport,
        static_cast<uint32_t>(viewports.size()),
        viewports.data()
    );
}

void CommandBuffer::setScissor(const std::vector<VkRect2D>& scissors, uint32_t firstScissor) const {
    vkCmdSetScissor(
        static_cast<VkCommandBuffer>(_commandBuffer),
        firstScissor,
        static_cast<uint32_t>(scissors.size()),
        scissors.data(
        ));
}

void CommandBuffer::setBlendConstants(const float blendConstants[4]) const {
    vkCmdSetBlendConstants(static_cast<VkCommandBuffer>(_commandBuffer), blendConstants);
}

void CommandBuffer::pushConstants(const CmdPushConstants& parameters) const {
    vkCmdPushConstants(
        static_cast<VkCommandBuffer>(_commandBuffer),
        parameters.layout,
        parameters.stageFlags,
        parameters.offset,
        parameters.size,
        parameters.values
    );
}

} // API
} // Vulkan
} // Graphics
} // lug
