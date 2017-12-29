#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Render/BloomPass.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

BloomPass::BloomPass(lug::Graphics::Vulkan::Renderer& renderer, lug::Graphics::Vulkan::Render::Window& window) : _renderer(renderer), _window(window) {}

BloomPass::~BloomPass() {
    destroy();
}

bool BloomPass::init() {
    auto& swapchainImages = _window.getSwapchain().getImages();
    uint32_t frameDataSize = (uint32_t)swapchainImages.size();
    API::Device& device = _renderer.getDevice();

    // Get transfer queue family
    {
        _transferQueue = device.getQueue("queue_transfer");
        if (!_transferQueue) {
            LUG_LOG.error("Gui::initFontsTexture: Can't find transfer queue");
            return false;
        }
    }

    // Create command pool
    {
        VkResult result{VK_SUCCESS};
        API::Builder::CommandPool commandPoolBuilder(device, *_transferQueue->getQueueFamily());
        if (!commandPoolBuilder.build(_commandPool, &result)) {
            LUG_LOG.error("Gui::init: Can't create a command pool: {}", result);
            return false;
        }
    }

    API::Builder::CommandBuffer commandBufferBuilder(_renderer.getDevice(), _commandPool);
    commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    API::Builder::Semaphore semaphoreBuilder(_renderer.getDevice());

    _framesData.resize(frameDataSize);
    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // Bloom finished semaphore
        {
            VkResult result{VK_SUCCESS};
            if (!semaphoreBuilder.build(_framesData[i].bloomFinishedSemaphores, &result)) {
                LUG_LOG.error("Window::initFramesData: Can't create semaphore: {}", result);
                return false;
            }
        }

        // Command buffers
        {
            VkResult result{VK_SUCCESS};
            if (!commandBufferBuilder.build(_framesData[i].cmdBuffer, &result)) {
                LUG_LOG.error("Window::initFramesData: Can't create the command buffer: {}", result);
                return false;
            }
        }
    }

    return buildEndCommandBuffer();
}

void BloomPass::destroy() {
    _framesData.clear();
    _commandPool.destroy();
}

bool BloomPass::endFrame(const std::vector<VkSemaphore>& waitSemaphores, uint32_t currentImageIndex) {
    FrameData& frameData = _framesData[currentImageIndex];

    std::vector<VkPipelineStageFlags> waitDstStageMasks(waitSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    if (_transferQueue->submit(frameData.cmdBuffer, { static_cast<VkSemaphore>(frameData.bloomFinishedSemaphores) }, waitSemaphores, waitDstStageMasks, nullptr) == false) {
        LUG_LOG.error("BloomPass::endFrame Can't submit commandBuffer");
        return false;
    }

    return true;
}

const Vulkan::API::Semaphore& BloomPass::getSemaphore(uint32_t currentImageIndex) const {
    return _framesData[currentImageIndex].bloomFinishedSemaphores;
}

bool BloomPass::buildEndCommandBuffer() {
    auto& swapchainImages = _window.getSwapchain().getImages();
    auto& sceneOffscreenImages = _window.getSceneOffscreenImages();
    auto& glowOffscreenImages = _window.getGlowOffscreenImages();
    uint32_t frameDataSize = (uint32_t)swapchainImages.size();

    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // Build command buffer image color attachment to present
        API::CommandBuffer& cmdBuffer = _framesData[i].cmdBuffer;

        if (!cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT)) {
            return false;
        }

        // Prepare window image for copying
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &swapchainImages[i];

            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }

        // Prepare offscreen images for copying
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &glowOffscreenImages[i];

            // Scene offscreen
            pipelineBarrier.imageMemoryBarriers[0].image = &sceneOffscreenImages[i];
            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            // Glow offscreen
            pipelineBarrier.imageMemoryBarriers[0].image = &glowOffscreenImages[i];
            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }

        // Copy offscreen image into window image
        {
            VkImageCopy copyRegion = {};

            copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.srcSubresource.baseArrayLayer = 0;
            copyRegion.srcSubresource.mipLevel = 0;
            copyRegion.srcSubresource.layerCount = 1;
            copyRegion.srcOffset = { 0, 0, 0 };

            copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.dstSubresource.baseArrayLayer = 0;
            copyRegion.dstSubresource.mipLevel = 0;
            copyRegion.dstSubresource.layerCount = 1;
            copyRegion.dstOffset = { 0, 0, 0 };

            copyRegion.extent.width = _window.getSwapchain().getExtent().width;
            copyRegion.extent.height = _window.getSwapchain().getExtent().height;
            copyRegion.extent.depth = 1;

            const API::CommandBuffer::CmdCopyImage cmdCopyImage{
                /* cmdCopyImage.srcImage      */ glowOffscreenImages[i],
                /* cmdCopyImage.srcImageLayout  */ VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                /* cmdCopyImage.dstImage      */ swapchainImages[i],
                /* cmdCopyImage.dsrImageLayout        */ VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                /* cmdCopyImage.regions      */ { copyRegion }
            };

            cmdBuffer.copyImage(cmdCopyImage);
        }

        // Prepare offscreen images for writing
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            // Scene offscreen
            pipelineBarrier.imageMemoryBarriers[0].image = &sceneOffscreenImages[i];
            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            // Glow offscreen
            pipelineBarrier.imageMemoryBarriers[0].image = &glowOffscreenImages[i];
            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }

        // Prepare window image for GUI writing
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &swapchainImages[i];

            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }

        if (!cmdBuffer.end()) {
            return false;
        }
    }

    return true;
}

} // Render
} // Vulkan
} // Graphics
} // lug
