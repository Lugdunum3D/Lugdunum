#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Image.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Semaphore.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Sampler.hpp>
#include <lug/Graphics/Vulkan/Render/BloomPass.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/BloomSampler.hpp>
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

    // Get transfer queue
    {
        _transferQueue = device.getQueue("queue_transfer");
        if (!_transferQueue) {
            LUG_LOG.error("BloomPass::init: Can't find transfer queue");
            return false;
        }
    }

    // Get transfer queue
    {
        _graphicsQueue = device.getQueue("queue_graphics");
        if (!_graphicsQueue) {
            LUG_LOG.error("BloomPass::init: Can't find graphics queue");
            return false;
        }
    }

    // Create command pools
    {
        // Transfer
        {
            VkResult result{VK_SUCCESS};
            API::Builder::CommandPool commandPoolBuilder(device, *_transferQueue->getQueueFamily());
            if (!commandPoolBuilder.build(_transferQueueCommandPool, &result)) {
                LUG_LOG.error("BloomPass::init: Can't create a transfer command pool: {}", result);
                return false;
            }
        }

        // Graphics
        {
            VkResult result{VK_SUCCESS};
            API::Builder::CommandPool commandPoolBuilder(device, *_graphicsQueue->getQueueFamily());
            if (!commandPoolBuilder.build(_graphicsCommandPool, &result)) {
                LUG_LOG.error("BloomPass::init: Can't create a graphics command pool: {}", result);
                return false;
            }
        }
    }

    API::Builder::CommandBuffer transferCommandBufferBuilder(device, _transferQueueCommandPool);
    transferCommandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    API::Builder::CommandBuffer graphicsCommandBufferBuilder(device, _transferQueueCommandPool);
    graphicsCommandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    API::Builder::Semaphore semaphoreBuilder(device);

    API::Builder::Fence fenceBuilder(device);
    fenceBuilder.setFlags(VK_FENCE_CREATE_SIGNALED_BIT); // Signaled state

    _framesData.resize(frameDataSize);
    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // Semaphores
        {
            VkResult result{VK_SUCCESS};
            if (!semaphoreBuilder.build(_framesData[i].bloomFinishedSemaphores, &result) ||
                !semaphoreBuilder.build(_framesData[i].glowCopyFinishedSemaphore, &result) ||
                !semaphoreBuilder.build(_framesData[i].blurFinishedSemaphore, &result)) {
                LUG_LOG.error("BloomPass::init: Can't create semaphores: {}", result);
                return false;
            }
        }

        // Fence
        {
            VkResult result{VK_SUCCESS};
            if (!fenceBuilder.build(_framesData[i].fence, &result)) {
                LUG_LOG.error("Window::init: Can't create fences: {}", result);
                return false;
            }
        }

        // Transfer command buffers
        {
            VkResult result{VK_SUCCESS};
            _framesData[i].transferCmdBuffers.resize(2);
            if (!transferCommandBufferBuilder.build(_framesData[i].transferCmdBuffers, &result)) {
                LUG_LOG.error("BloomPass::init: Can't create command buffers: {}", result);
                return false;
            }
        }

        // Graphics command buffer
        {
            VkResult result{VK_SUCCESS};
            if (!graphicsCommandBufferBuilder.build(_framesData[i].graphicsCmdBuffer, &result)) {
                LUG_LOG.error("BloomPass::init: Can't create the command buffer: {}", result);
                return false;
            }
        }
    }

    _texturesDescriptorSetPool = std::make_unique<Render::DescriptorSetPool::BloomSampler>(_renderer);
    if (!_texturesDescriptorSetPool->init()) {
        return false;
    }

    return initPipelines() && initBlurPass() && buildEndCommandBuffer();
}

void BloomPass::destroy() {
    _horizontalPipeline.destroy();
    _verticalPipeline.destroy();

    for (const auto& frameData: _framesData) {
        for (const auto& descriptorSet : frameData.texturesDescriptorSets) {
            _texturesDescriptorSetPool->free(descriptorSet);
        }
    }

    _framesData.clear();

    _texturesDescriptorSetPool.reset();

    _transferQueueCommandPool.destroy();
    _graphicsCommandPool.destroy();
}

bool BloomPass::endFrame(const std::vector<VkSemaphore>& waitSemaphores, uint32_t currentImageIndex) {
    FrameData& frameData = _framesData[currentImageIndex];

    // transferCmdBuffers[0] copy the glow image into the first blur images
    // Then, the layout of the blur images are changed for the first blur pass
    {
        std::vector<VkPipelineStageFlags> waitDstStageMasks(waitSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        if (_transferQueue->submit(frameData.transferCmdBuffers[0], { static_cast<VkSemaphore>(frameData.glowCopyFinishedSemaphore) }, waitSemaphores, waitDstStageMasks, nullptr) == false) {
            LUG_LOG.error("BloomPass::endFrame Can't submit commandBuffer");
            return false;
        }
    }

    if (!renderBlurPass(currentImageIndex)) {
        return false;
    }

    // Change the window image layout for GUi draw
    // Change blur images layout for next frame
    {
        std::vector<VkPipelineStageFlags> waitDstStageMasks(waitSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        if (_transferQueue->submit(frameData.transferCmdBuffers[1], { static_cast<VkSemaphore>(frameData.bloomFinishedSemaphores) }, {static_cast<VkSemaphore>(frameData.blurFinishedSemaphore)}, waitDstStageMasks, nullptr) == false) {
            LUG_LOG.error("BloomPass::endFrame Can't submit commandBuffer");
            return false;
        }
    }

    return true;
}

const Vulkan::API::Semaphore& BloomPass::getSemaphore(uint32_t currentImageIndex) const {
    return _framesData[currentImageIndex].bloomFinishedSemaphores;
}

bool BloomPass::renderBlurPass(uint32_t currentImageIndex) {
    FrameData& frameData = _framesData[currentImageIndex];

    if (!frameData.fence.wait()) {
        return false;
    }
    frameData.fence.reset();

    if (frameData.freeDescriptorSets) {
        for (const auto& descriptorSet : frameData.texturesDescriptorSets) {
            _texturesDescriptorSetPool->free(descriptorSet);
        }
        frameData.freeDescriptorSets = false;
        frameData.texturesDescriptorSets.clear();
    }

    frameData.graphicsCmdBuffer.reset();
    frameData.graphicsCmdBuffer.begin();

    // Temporary array of descriptor sets use to render this frame
    // they will replace frameData.texturesDescriptorSets atfer the rendering
    std::vector<const Render::DescriptorSetPool::DescriptorSet*> texturesDescriptorSets;

    // Blur passes
    {
        for (auto& blurPass: frameData.blurPasses) {
            // Set viewport/scissor
            {
                const VkViewport vkViewport{
                    /* vkViewport.x         */ 0.0f,
                    /* vkViewport.y         */ 0.0f,
                    /* vkViewport.width     */ static_cast<float>(blurPass.images[0].getExtent().width),
                    /* vkViewport.height    */ static_cast<float>(blurPass.images[0].getExtent().height),
                    /* vkViewport.minDepth  */ 0.0f,
                    /* vkViewport.maxDepth  */ 1.0f,
                };

                const VkRect2D scissor{
                    /* scissor.offset */ {
                       0,
                       0
                    },
                    /* scissor.extent */ {
                       static_cast<uint32_t>(vkViewport.width),
                       static_cast<uint32_t>(vkViewport.height)
                    }
                };

                frameData.graphicsCmdBuffer.setViewport({vkViewport});
                frameData.graphicsCmdBuffer.setScissor({scissor});
            }

            // Horizontal pass
            {
                const API::RenderPass* renderPass = _horizontalPipeline.getRenderPass();

                API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
                    /* beginRenderPass.framebuffer */ blurPass.framebuffers[1],
                    /* beginRenderPass.renderArea */{},
                    /* beginRenderPass.clearValues */{}
                };

                beginRenderPass.renderArea.offset = { 0, 0 };
                beginRenderPass.renderArea.extent = { blurPass.images[0].getExtent().width, blurPass.images[0].getExtent().height };

                beginRenderPass.clearValues.resize(1);
                beginRenderPass.clearValues[0].color = {{0.0f, 0.0f, 0.0f, 0.0f}};


                frameData.graphicsCmdBuffer.beginRenderPass(*renderPass, beginRenderPass);
                frameData.graphicsCmdBuffer.bindPipeline(_horizontalPipeline);



                // Get the new (or old) textures descriptor set
                const Render::DescriptorSetPool::DescriptorSet* texturesDescriptorSet = _texturesDescriptorSetPool->allocate(
                    _horizontalPipeline,
                    blurPass.images[0],
                    blurPass.imagesViews[0],
                    blurPass.samplers[0]
                );

                if (!texturesDescriptorSet) {
                    LUG_LOG.error("BloomPass::renderBlurPass: Can't allocate textures descriptor set");
                    return false;
                }

                texturesDescriptorSets.push_back(texturesDescriptorSet);

                // Bind descriptor set of the texture
                {
                    const API::CommandBuffer::CmdBindDescriptors texturesBind{
                        /* texturesBind.pipelineLayout     */ *_horizontalPipeline.getLayout(),
                        /* texturesBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                        /* texturesBind.firstSet           */ 0,
                        /* texturesBind.descriptorSets     */ {&texturesDescriptorSet->getDescriptorSet()},
                        /* texturesBind.dynamicOffsets     */ {}
                    };

                    frameData.graphicsCmdBuffer.bindDescriptorSets(texturesBind);
                }

                const API::CommandBuffer::CmdDraw cmdDraw {
                    /* cmdDraw.vertexCount */ 3,
                    /* cmdDraw.instanceCount */ 1,
                    /* cmdDraw.firstVertex */ 0,
                    /* cmdDraw.firstInstance */ 0
                };

                frameData.graphicsCmdBuffer.draw(cmdDraw);
                frameData.graphicsCmdBuffer.endRenderPass();
            }

            // Change images layout for vertical pass
            {
                // Prepare blur[0] image for write
                {
                    API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                    pipelineBarrier.imageMemoryBarriers.resize(1);
                    pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[0];

                    frameData.graphicsCmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
                }

                // Prepare blur[1] image for read in shader
                {
                    API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                    pipelineBarrier.imageMemoryBarriers.resize(1);
                    pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[1];

                    frameData.graphicsCmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
                }
            }

            // Vertical pass
            {
                const API::RenderPass* renderPass = _verticalPipeline.getRenderPass();

                API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
                    /* beginRenderPass.framebuffer */ blurPass.framebuffers[0],
                    /* beginRenderPass.renderArea */{},
                    /* beginRenderPass.clearValues */{}
                };

                beginRenderPass.renderArea.offset = { 0, 0 };
                beginRenderPass.renderArea.extent = { blurPass.images[0].getExtent().width, blurPass.images[0].getExtent().height };

                beginRenderPass.clearValues.resize(1);
                beginRenderPass.clearValues[0].color = {{0.0f, 0.0f, 0.0f, 0.0f}};

                frameData.graphicsCmdBuffer.beginRenderPass(*renderPass, beginRenderPass);
                frameData.graphicsCmdBuffer.bindPipeline(_verticalPipeline);



                // Get the new (or old) textures descriptor set
                const Render::DescriptorSetPool::DescriptorSet* texturesDescriptorSet = _texturesDescriptorSetPool->allocate(
                    _verticalPipeline,
                    blurPass.images[1],
                    blurPass.imagesViews[1],
                    blurPass.samplers[1]
                );

                if (!texturesDescriptorSet) {
                    LUG_LOG.error("BloomPass::renderBlurPass: Can't allocate textures descriptor set");
                    return false;
                }

                texturesDescriptorSets.push_back(texturesDescriptorSet);

                // Bind descriptor set of the texture
                {
                    const API::CommandBuffer::CmdBindDescriptors texturesBind{
                        /* texturesBind.pipelineLayout     */ *_verticalPipeline.getLayout(),
                        /* texturesBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                        /* texturesBind.firstSet           */ 0,
                        /* texturesBind.descriptorSets     */ {&texturesDescriptorSet->getDescriptorSet()},
                        /* texturesBind.dynamicOffsets     */ {}
                    };

                    frameData.graphicsCmdBuffer.bindDescriptorSets(texturesBind);
                }

                const API::CommandBuffer::CmdDraw cmdDraw {
                    /* cmdDraw.vertexCount */ 3,
                    /* cmdDraw.instanceCount */ 1,
                    /* cmdDraw.firstVertex */ 0,
                    /* cmdDraw.firstInstance */ 0
                };

                frameData.graphicsCmdBuffer.draw(cmdDraw);
                frameData.graphicsCmdBuffer.endRenderPass();
            }

            // Prepare blur[0] image for read in shader
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[0];

                frameData.graphicsCmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            }
        }
    }

    // Change images layout for blend pass
    {

        // Prepare swapchain image for read in shader
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &_window.getSwapchain().getImages()[currentImageIndex];

            frameData.graphicsCmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }
    }

    // Set viewport/scissor
    {
        const VkViewport vkViewport{
            /* vkViewport.x         */ 0.0f,
            /* vkViewport.y         */ 0.0f,
            /* vkViewport.width     */ static_cast<float>(frameData.blendPass.image.getExtent().width),
            /* vkViewport.height    */ static_cast<float>(frameData.blendPass.image.getExtent().height),
            /* vkViewport.minDepth  */ 0.0f,
            /* vkViewport.maxDepth  */ 1.0f,
        };

        const VkRect2D scissor{
            /* scissor.offset */ {
               0,
               0
            },
            /* scissor.extent */ {
               static_cast<uint32_t>(vkViewport.width),
               static_cast<uint32_t>(vkViewport.height)
            }
        };

        frameData.graphicsCmdBuffer.setViewport({vkViewport});
        frameData.graphicsCmdBuffer.setScissor({scissor});
    }

    // Blend pass
    {
        auto& swapchain = _window.getSwapchain();
        const API::RenderPass* renderPass = _blendPipeline.getRenderPass();

        API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
            /* beginRenderPass.framebuffer */ frameData.blendPass.framebuffer,
            /* beginRenderPass.renderArea */{},
            /* beginRenderPass.clearValues */{}
        };

        beginRenderPass.renderArea.offset = { 0, 0 };
        beginRenderPass.renderArea.extent = { frameData.blendPass.image.getExtent().width, frameData.blendPass.image.getExtent().height };

        frameData.graphicsCmdBuffer.beginRenderPass(*renderPass, beginRenderPass);
        frameData.graphicsCmdBuffer.bindPipeline(_blendPipeline);

        // Draw window image
         {
            // Get the new (or old) textures descriptor set
            const Render::DescriptorSetPool::DescriptorSet* texturesDescriptorSet = _texturesDescriptorSetPool->allocate(
                _blendPipeline,
                swapchain.getImages()[currentImageIndex],
                swapchain.getImagesViews()[currentImageIndex],
                frameData.blendPass.sampler
            );

            if (!texturesDescriptorSet) {
                LUG_LOG.error("BloomPass::renderBlurPass: Can't allocate textures descriptor set");
                return false;
            }

            texturesDescriptorSets.push_back(texturesDescriptorSet);

            // Bind descriptor set of the texture
            {
                const API::CommandBuffer::CmdBindDescriptors texturesBind{
                    /* texturesBind.pipelineLayout     */ *_blendPipeline.getLayout(),
                    /* texturesBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                    /* texturesBind.firstSet           */ 0,
                    /* texturesBind.descriptorSets     */ {&texturesDescriptorSet->getDescriptorSet()},
                    /* texturesBind.dynamicOffsets     */ {}
                };

                frameData.graphicsCmdBuffer.bindDescriptorSets(texturesBind);
            }

            const API::CommandBuffer::CmdDraw cmdDraw {
                /* cmdDraw.vertexCount */ 3,
                /* cmdDraw.instanceCount */ 1,
                /* cmdDraw.firstVertex */ 0,
                /* cmdDraw.firstInstance */ 0
            };

            frameData.graphicsCmdBuffer.draw(cmdDraw);
        }

        // Draw blur images
         {
            for (auto& blurPass: frameData.blurPasses) {
                // Get the new (or old) textures descriptor set
                const Render::DescriptorSetPool::DescriptorSet* texturesDescriptorSet = _texturesDescriptorSetPool->allocate(
                    _blendPipeline,
                    blurPass.images[0],
                    blurPass.imagesViews[0],
                    blurPass.samplers[0]
                );

                if (!texturesDescriptorSet) {
                    LUG_LOG.error("BloomPass::renderBlurPass: Can't allocate textures descriptor set");
                    return false;
                }

                texturesDescriptorSets.push_back(texturesDescriptorSet);

                // Bind descriptor set of the texture
                {
                    const API::CommandBuffer::CmdBindDescriptors texturesBind{
                        /* texturesBind.pipelineLayout     */ *_blendPipeline.getLayout(),
                        /* texturesBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                        /* texturesBind.firstSet           */ 0,
                        /* texturesBind.descriptorSets     */ {&texturesDescriptorSet->getDescriptorSet()},
                        /* texturesBind.dynamicOffsets     */ {}
                    };

                    frameData.graphicsCmdBuffer.bindDescriptorSets(texturesBind);
                }

                const API::CommandBuffer::CmdDraw cmdDraw {
                    /* cmdDraw.vertexCount */ 3,
                    /* cmdDraw.instanceCount */ 1,
                    /* cmdDraw.firstVertex */ 0,
                    /* cmdDraw.firstInstance */ 0
                };

                frameData.graphicsCmdBuffer.draw(cmdDraw);
            }
        }

        frameData.graphicsCmdBuffer.endRenderPass();
    }

    // Change images layout for hdr pass
    {
        // Prepare blend image for read in shader
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &frameData.blendPass.image;

            frameData.graphicsCmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }

        // Prepare swapchain image for write
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &_window.getSwapchain().getImages()[currentImageIndex];

            frameData.graphicsCmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }
    }

    // Set viewport/scissor
    {
        const VkViewport vkViewport{
            /* vkViewport.x         */ 0.0f,
            /* vkViewport.y         */ 0.0f,
            /* vkViewport.width     */ static_cast<float>(_window.getSwapchain().getExtent().width),
            /* vkViewport.height    */ static_cast<float>(_window.getSwapchain().getExtent().height),
            /* vkViewport.minDepth  */ 0.0f,
            /* vkViewport.maxDepth  */ 1.0f,
        };

        const VkRect2D scissor{
            /* scissor.offset */ {
               0,
               0
            },
            /* scissor.extent */ {
               static_cast<uint32_t>(vkViewport.width),
               static_cast<uint32_t>(vkViewport.height)
            }
        };

        frameData.graphicsCmdBuffer.setViewport({vkViewport});
        frameData.graphicsCmdBuffer.setScissor({scissor});
    }

    // Hdr pass
    {
        auto& swapchain = _window.getSwapchain();
        const API::RenderPass* renderPass = _hdrPipeline.getRenderPass();

        API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
            /* beginRenderPass.framebuffer */ frameData.hdrFramebuffer,
            /* beginRenderPass.renderArea */{},
            /* beginRenderPass.clearValues */{}
        };

        beginRenderPass.renderArea.offset = { 0, 0 };
        beginRenderPass.renderArea.extent = { swapchain.getImages()[0].getExtent().width, swapchain.getImages()[0].getExtent().height };

        beginRenderPass.clearValues.resize(1);
        beginRenderPass.clearValues[0].color = {{0.0f, 0.0f, 0.0f, 0.0f}};

        frameData.graphicsCmdBuffer.beginRenderPass(*renderPass, beginRenderPass);
        frameData.graphicsCmdBuffer.bindPipeline(_hdrPipeline);

        // Get the new (or old) textures descriptor set
        const Render::DescriptorSetPool::DescriptorSet* texturesDescriptorSet = _texturesDescriptorSetPool->allocate(
            _hdrPipeline,
            frameData.blendPass.image,
            frameData.blendPass.imageView,
            frameData.blendPass.sampler
        );

        if (!texturesDescriptorSet) {
            LUG_LOG.error("BloomPass::renderBlurPass: Can't allocate textures descriptor set");
            return false;
        }

        texturesDescriptorSets.push_back(texturesDescriptorSet);

        // Bind descriptor set of the texture
        {
            const API::CommandBuffer::CmdBindDescriptors texturesBind{
                /* texturesBind.pipelineLayout     */ *_hdrPipeline.getLayout(),
                /* texturesBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                /* texturesBind.firstSet           */ 0,
                /* texturesBind.descriptorSets     */ {&texturesDescriptorSet->getDescriptorSet()},
                /* texturesBind.dynamicOffsets     */ {}
            };

            frameData.graphicsCmdBuffer.bindDescriptorSets(texturesBind);
        }

        const API::CommandBuffer::CmdDraw cmdDraw {
            /* cmdDraw.vertexCount */ 3,
            /* cmdDraw.instanceCount */ 1,
            /* cmdDraw.firstVertex */ 0,
            /* cmdDraw.firstInstance */ 0
        };

        frameData.graphicsCmdBuffer.draw(cmdDraw);
        frameData.graphicsCmdBuffer.endRenderPass();
    }

    frameData.graphicsCmdBuffer.end();

    // Free and replace previous texturesDescriptorSets
    {
        for (const auto& descriptorSet : frameData.texturesDescriptorSets) {
            _texturesDescriptorSetPool->free(descriptorSet);
        }

        frameData.texturesDescriptorSets = texturesDescriptorSets;
    }


    std::vector<VkPipelineStageFlags> waitDstStageMasks{VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
    if (_graphicsQueue->submit(frameData.graphicsCmdBuffer, { static_cast<VkSemaphore>(frameData.blurFinishedSemaphore) }, {static_cast<VkSemaphore>(frameData.glowCopyFinishedSemaphore)}, waitDstStageMasks, static_cast<VkFence>(frameData.fence)) == false) {
        LUG_LOG.error("BloomPass::renderBlurPass: Can't submit commandBuffer");
        return false;
    }

    return true;
}

bool BloomPass::initHdrPipeline() {
 API::Device &device = _renderer.getDevice();

    API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

    // Set shaders state
    if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", _renderer.getInfo().shadersRoot + "fullscreen-quad.vert.spv")
        || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", _renderer.getInfo().shadersRoot + "hdr.frag.spv")) {
        LUG_LOG.error("BloomPass::initHdrPipeline: Can't create pipeline's shaders.");
        return false;
    }

    // Set viewport state
    const VkViewport viewport{
        /* viewport.x */ 0.0f,
        /* viewport.y */ 0.0f,
        /* viewport.width */ 0.0f,
        /* viewport.height */ 0.0f,
        /* viewport.minDepth */ 0.0f,
        /* viewport.maxDepth */ 1.0f,
    };

    const VkRect2D scissor{
        /* scissor.offset */{ 0, 0 },
        /* scissor.extent */{ 0, 0 }
    };

    auto viewportState = graphicsPipelineBuilder.getViewportState();
    viewportState.addViewport(viewport);
    viewportState.addScissor(scissor);

    // Set rasterization state
    auto rasterizationState = graphicsPipelineBuilder.getRasterizationState();
    rasterizationState.setCullMode(VK_CULL_MODE_NONE);

    // Set color blend state
    const VkPipelineColorBlendAttachmentState colorBlendAttachment{
        /* colorBlendAttachment.blendEnable */ VK_TRUE,
        /* colorBlendAttachment.srcColorBlendFactor */ VK_BLEND_FACTOR_SRC_ALPHA,
        /* colorBlendAttachment.dstColorBlendFactor */ VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        /* colorBlendAttachment.colorBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_ZERO,
        /* colorBlendAttachment.alphaBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.colorWriteMask */ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    auto colorBlendState = graphicsPipelineBuilder.getColorBlendState();
    colorBlendState.addAttachment(colorBlendAttachment);

    // Set dynamic states
    graphicsPipelineBuilder.setDynamicStates({
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    });

    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts;
    {
        // descriptorSetLayout
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(device);

            // Blur texture
            const VkDescriptorSetLayoutBinding binding{
                /* binding.binding */ 0,
                /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                /* binding.descriptorCount */ 1,
                /* binding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                /* binding.pImmutableSamplers */ nullptr
            };

            descriptorSetLayoutBuilder.setBindings({ binding });

            // create descriptor set
            VkResult result{VK_SUCCESS};
            descriptorSetLayouts.resize(1);
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("BloomPass::initHdrPipeline: Can't create pipeline descriptor: {}", result);
                return false;
            }
        }

        API::Builder::PipelineLayout pipelineLayoutBuilder(device);

        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        API::PipelineLayout pipelineLayout;
        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("BloomPass::initHdrPipeline: Can't create pipeline layout: {}", result);
            return false;
        }
        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

        auto colorFormat = _window.getSwapchain().getFormat().format;

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ colorFormat,
            /* colorAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* colorAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_CLEAR,
            /* colorAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* colorAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* colorAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* colorAttachment.initialLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            /* colorAttachment.finalLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        const API::Builder::RenderPass::SubpassDescription subpassDescription{
            /* subpassDescription.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* subpassDescription.inputAttachments */{},
            /* subpassDescription.colorAttachments */{ { colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
            /* subpassDescription.resolveAttachments */{},
            /* subpassDescription.depthStencilAttachment */{},
            /* subpassDescription.preserveAttachments */{},
        };

        renderPassBuilder.addSubpass(subpassDescription);

        VkResult result{VK_SUCCESS};
        API::RenderPass renderPass;
        if (!renderPassBuilder.build(renderPass, &result)) {
            LUG_LOG.error("BloomPass::initHdrPipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    // Create pipeline
    {
        VkResult result{VK_SUCCESS};
        if (!graphicsPipelineBuilder.build(_hdrPipeline, &result)) {
            LUG_LOG.error("BloomPass::initHdrPipeline: Can't create pipeline: {}", result);
            return false;
        }
    }

    return true;
}

bool BloomPass::initBlendPipeline() {
 API::Device &device = _renderer.getDevice();

    API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

    // Set shaders state
    if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", _renderer.getInfo().shadersRoot + "fullscreen-quad.vert.spv")
        || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", _renderer.getInfo().shadersRoot + "blur-blend.frag.spv")) {
        LUG_LOG.error("BloomPass::initBlendPipeline: Can't create pipeline's shaders.");
        return false;
    }

    // Set viewport state
    const VkViewport viewport{
        /* viewport.x */ 0.0f,
        /* viewport.y */ 0.0f,
        /* viewport.width */ 0.0f,
        /* viewport.height */ 0.0f,
        /* viewport.minDepth */ 0.0f,
        /* viewport.maxDepth */ 1.0f,
    };

    const VkRect2D scissor{
        /* scissor.offset */{ 0, 0 },
        /* scissor.extent */{ 0, 0 }
    };

    auto viewportState = graphicsPipelineBuilder.getViewportState();
    viewportState.addViewport(viewport);
    viewportState.addScissor(scissor);

    // Set rasterization state
    auto rasterizationState = graphicsPipelineBuilder.getRasterizationState();
    rasterizationState.setCullMode(VK_CULL_MODE_NONE);

    // Set color blend state
    const VkPipelineColorBlendAttachmentState colorBlendAttachment{
        /* colorBlendAttachment.blendEnable */ VK_TRUE,
        /* colorBlendAttachment.srcColorBlendFactor */ VK_BLEND_FACTOR_ONE,
        /* colorBlendAttachment.dstColorBlendFactor */ VK_BLEND_FACTOR_ONE,
        /* colorBlendAttachment.colorBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_ZERO,
        /* colorBlendAttachment.alphaBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.colorWriteMask */ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    auto colorBlendState = graphicsPipelineBuilder.getColorBlendState();
    colorBlendState.addAttachment(colorBlendAttachment);

    // Set dynamic states
    graphicsPipelineBuilder.setDynamicStates({
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    });

    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts;
    {
        // descriptorSetLayout
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(device);

            // Blur texture
            const VkDescriptorSetLayoutBinding binding{
                /* binding.binding */ 0,
                /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                /* binding.descriptorCount */ 1,
                /* binding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                /* binding.pImmutableSamplers */ nullptr
            };

            descriptorSetLayoutBuilder.setBindings({ binding });

            // create descriptor set
            VkResult result{VK_SUCCESS};
            descriptorSetLayouts.resize(1);
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("BloomPass::initBlendPipeline: Can't create pipeline descriptor: {}", result);
                return false;
            }
        }

        API::Builder::PipelineLayout pipelineLayoutBuilder(device);

        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        API::PipelineLayout pipelineLayout;
        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("BloomPass::initBlendPipeline: Can't create pipeline layout: {}", result);
            return false;
        }
        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

        auto colorFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ colorFormat,
            /* colorAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* colorAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_LOAD,
            /* colorAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* colorAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* colorAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* colorAttachment.initialLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            /* colorAttachment.finalLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        const API::Builder::RenderPass::SubpassDescription subpassDescription{
            /* subpassDescription.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* subpassDescription.inputAttachments */{},
            /* subpassDescription.colorAttachments */{ { colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
            /* subpassDescription.resolveAttachments */{},
            /* subpassDescription.depthStencilAttachment */{},
            /* subpassDescription.preserveAttachments */{},
        };

        renderPassBuilder.addSubpass(subpassDescription);

        VkResult result{VK_SUCCESS};
        API::RenderPass renderPass;
        if (!renderPassBuilder.build(renderPass, &result)) {
            LUG_LOG.error("BloomPass::initBlendPipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    // Create pipeline
    {
        VkResult result{VK_SUCCESS};
        if (!graphicsPipelineBuilder.build(_blendPipeline, &result)) {
            LUG_LOG.error("BloomPass::initBlendPipeline: Can't create pipeline: {}", result);
            return false;
        }
    }

    return true;
}

bool BloomPass::initBlurPipeline(API::GraphicsPipeline& pipeline, int blurDirection) {
 API::Device &device = _renderer.getDevice();

    API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

    const VkSpecializationMapEntry specializationEntry{
        /* constantID   */ 0,
        /* offset       */ 0,
        /* size         */ sizeof(int)
    };
    VkSpecializationInfo specializationInfo{
        /* mapEntryCount    */ 1,
        /* pMapEntries      */ {},
        /* dataSize         */ sizeof(int),
        /* pData            */ &blurDirection
    };
    specializationInfo.pMapEntries = &specializationEntry;

    // Set shaders state
    if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", _renderer.getInfo().shadersRoot + "fullscreen-quad.vert.spv")
        || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", _renderer.getInfo().shadersRoot + "blur.frag.spv", &specializationInfo)) {
        LUG_LOG.error("BloomPass::initBlurPipeline: Can't create pipeline's shaders.");
        return false;
    }

    // Set viewport state
    const VkViewport viewport{
        /* viewport.x */ 0.0f,
        /* viewport.y */ 0.0f,
        /* viewport.width */ 0.0f,
        /* viewport.height */ 0.0f,
        /* viewport.minDepth */ 0.0f,
        /* viewport.maxDepth */ 1.0f,
    };

    const VkRect2D scissor{
        /* scissor.offset */{ 0, 0 },
        /* scissor.extent */{ 0, 0 }
    };

    auto viewportState = graphicsPipelineBuilder.getViewportState();
    viewportState.addViewport(viewport);
    viewportState.addScissor(scissor);

    // Set rasterization state
    auto rasterizationState = graphicsPipelineBuilder.getRasterizationState();
    rasterizationState.setCullMode(VK_CULL_MODE_NONE);

    // Set color blend state
    const VkPipelineColorBlendAttachmentState colorBlendAttachment{
        /* colorBlendAttachment.blendEnable */ VK_TRUE,
        /* colorBlendAttachment.srcColorBlendFactor */ VK_BLEND_FACTOR_SRC_ALPHA,
        /* colorBlendAttachment.dstColorBlendFactor */ VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        /* colorBlendAttachment.colorBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_ZERO,
        /* colorBlendAttachment.alphaBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.colorWriteMask */ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    auto colorBlendState = graphicsPipelineBuilder.getColorBlendState();
    colorBlendState.addAttachment(colorBlendAttachment);

    // Set dynamic states
    graphicsPipelineBuilder.setDynamicStates({
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    });

    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts;
    {
        // descriptorSetLayout
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(device);

            // Blur texture
            const VkDescriptorSetLayoutBinding binding{
                /* binding.binding */ 0,
                /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                /* binding.descriptorCount */ 1,
                /* binding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                /* binding.pImmutableSamplers */ nullptr
            };

            descriptorSetLayoutBuilder.setBindings({ binding });

            // create descriptor set
            VkResult result{VK_SUCCESS};
            descriptorSetLayouts.resize(1);
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("BloomPass::initBlurPipeline: Can't create pipeline descriptor: {}", result);
                return false;
            }
        }

        API::Builder::PipelineLayout pipelineLayoutBuilder(device);

        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        API::PipelineLayout pipelineLayout;
        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("BloomPass::initBlurPipeline: Can't create pipeline layout: {}", result);
            return false;
        }
        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

        auto colorFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ colorFormat,
            /* colorAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* colorAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_CLEAR,
            /* colorAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* colorAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* colorAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* colorAttachment.initialLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            /* colorAttachment.finalLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        const API::Builder::RenderPass::SubpassDescription subpassDescription{
            /* subpassDescription.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* subpassDescription.inputAttachments */{},
            /* subpassDescription.colorAttachments */{ { colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
            /* subpassDescription.resolveAttachments */{},
            /* subpassDescription.depthStencilAttachment */{},
            /* subpassDescription.preserveAttachments */{},
        };

        renderPassBuilder.addSubpass(subpassDescription);

        VkResult result{VK_SUCCESS};
        API::RenderPass renderPass;
        if (!renderPassBuilder.build(renderPass, &result)) {
            LUG_LOG.error("BloomPass::initBlurPipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    // Create pipeline
    {
        VkResult result{VK_SUCCESS};
        if (!graphicsPipelineBuilder.build(pipeline, &result)) {
            LUG_LOG.error("BloomPass::initBlurPipeline: Can't create pipeline: {}", result);
            return false;
        }
    }

    return true;
}

bool BloomPass::initPipelines() {
    return initBlurPipeline(_horizontalPipeline, 1) && initBlurPipeline(_verticalPipeline, 0) && initBlendPipeline() && initHdrPipeline();
}

bool BloomPass::initBlurPass() {
    auto& swapchain = _window.getSwapchain();
    auto& swapchainImages = swapchain.getImages();
    auto& swapchainImagesViews = swapchain.getImagesViews();
    uint32_t frameDataSize = (uint32_t)swapchainImages.size();
    API::Device& device = _renderer.getDevice();

    API::Builder::DeviceMemory deviceMemoryBuilder(device);
    deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // Use the first command buffer and reset it after
    auto& cmdBuffer = _framesData[0].transferCmdBuffers[0];

    if (!cmdBuffer.begin()) {
        LUG_LOG.error("BloomPass::initBlurPass: Can't begin command buffer");
        return false;
    }

    // Free all descriptor sets after next frameData.fence.wait()
    for (auto& frameData: _framesData) {
        frameData.freeDescriptorSets = true;
    }

    // Create images
    {
        API::Builder::Image imageBuilder(device);

        imageBuilder.setUsage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        imageBuilder.setPreferedFormats({VK_FORMAT_R16G16B16A16_SFLOAT });
        imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        imageBuilder.setQueueFamilyIndices({ _transferQueue->getQueueFamily()->getIdx(), _graphicsQueue->getQueueFamily()->getIdx() });
        imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);

        for (uint8_t i = 0; i < frameDataSize; ++i) {
            // Blur images
            {
                _framesData[i].blurPasses.resize(3);
                // 0
                {
                    VkExtent3D extent{
                        /* extent.width */ static_cast<uint32_t>(swapchain.getExtent().width),
                        /* extent.height */ static_cast<uint32_t>(swapchain.getExtent().height),
                        /* extent.depth */ 1
                    };
                    imageBuilder.setExtent(extent);

                    VkResult result{VK_SUCCESS};
                    if (!imageBuilder.build(_framesData[i].blurPasses[0].images[0], &result) ||
                        !imageBuilder.build(_framesData[i].blurPasses[0].images[1], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur offscreen images: {}", result);
                        return false;
                    }
                }

                // 1
                {
                    VkExtent3D extent{
                        /* extent.width */ static_cast<uint32_t>(swapchain.getExtent().width) / 2,
                        /* extent.height */ static_cast<uint32_t>(swapchain.getExtent().height) / 2,
                        /* extent.depth */ 1
                    };
                    imageBuilder.setExtent(extent);

                    VkResult result{VK_SUCCESS};
                    if (!imageBuilder.build(_framesData[i].blurPasses[1].images[0], &result) ||
                        !imageBuilder.build(_framesData[i].blurPasses[1].images[1], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur offscreen images: {}", result);
                        return false;
                    }
                }

                // 2
                {
                    VkExtent3D extent{
                        /* extent.width */ static_cast<uint32_t>(swapchain.getExtent().width) / 3,
                        /* extent.height */ static_cast<uint32_t>(swapchain.getExtent().height) / 3,
                        /* extent.depth */ 1
                    };
                    imageBuilder.setExtent(extent);

                    VkResult result{VK_SUCCESS};
                    if (!imageBuilder.build(_framesData[i].blurPasses[2].images[0], &result) ||
                        !imageBuilder.build(_framesData[i].blurPasses[2].images[1], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur offscreen images: {}", result);
                        return false;
                    }
                }
            }

            // Blend image
            {
                VkExtent3D extent{
                    /* extent.width */ swapchain.getExtent().width,
                    /* extent.height */ swapchain.getExtent().height,
                    /* extent.depth */ 1
                };
                imageBuilder.setExtent(extent);

                VkResult result{VK_SUCCESS};
                if (!imageBuilder.build(_framesData[i].blendPass.image, &result)) {
                    LUG_LOG.error("BloomPass::initBlurPass: Can't create blend offscreen images: {}", result);
                    return false;
                }
            }

            // Add images to device memory
            {
                if (!deviceMemoryBuilder.addImage(_framesData[i].blendPass.image)) {
                    LUG_LOG.error("BloomPass::initBlurPass: Can't add offscreen images to device memory");
                    return false;
                }

                for (auto& blurPass: _framesData[i].blurPasses) {
                    if (!deviceMemoryBuilder.addImage(blurPass.images[0]) ||
                        !deviceMemoryBuilder.addImage(blurPass.images[1])) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't add offscreen images to device memory");
                        return false;
                    }
                }
            }

            // Change images layout
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;

                // Blur images
                {
                    for (auto& blurPass: _framesData[i].blurPasses) {
                        // blur[0]
                        pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                        pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                        pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[0];
                        cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);

                        // blur[1]
                        pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                        pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                        pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[1];
                        cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
                    }
                }

                // Blend
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &_framesData[i].blendPass.image;
                cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            }
        }
    }

    if (!cmdBuffer.end()) {
        LUG_LOG.error("BloomPass::initBlurPass: Can't end command buffer");
        return false;
    }

    // Create offscreen image memory
    {
        VkResult result{VK_SUCCESS};
        if (!deviceMemoryBuilder.build(_offscreenImagesMemory, &result)) {
            LUG_LOG.error("Vulkan::Texture::build: Can't create buffer device memory: {}", result);
            return false;
        }
    }

    // Create offscreen image views
    {
        for (uint8_t i = 0; i < frameDataSize; ++i) {
            for (auto& blurPass: _framesData[i].blurPasses) {
                // blur[0] image view
                {
                    VkResult result{VK_SUCCESS};
                    API::Builder::ImageView imageViewBuilder(device, blurPass.images[0]);

                    imageViewBuilder.setFormat(VK_FORMAT_R16G16B16A16_SFLOAT);

                    if (!imageViewBuilder.build(blurPass.imagesViews[0], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur[0] offscreen image view: {}", result);
                        return false;
                    }
                }

                // blur[1] image view
                {
                    VkResult result{VK_SUCCESS};
                    API::Builder::ImageView imageViewBuilder(device, blurPass.images[1]);

                    imageViewBuilder.setFormat(VK_FORMAT_R16G16B16A16_SFLOAT);

                    if (!imageViewBuilder.build(blurPass.imagesViews[1], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur[1] offscreen image view: {}", result);
                        return false;
                    }
                }
            }

            // Blend image view
            {
                VkResult result{VK_SUCCESS};
                API::Builder::ImageView imageViewBuilder(device, _framesData[i].blendPass.image);

                imageViewBuilder.setFormat(VK_FORMAT_R16G16B16A16_SFLOAT);

                if (!imageViewBuilder.build(_framesData[i].blendPass.imageView, &result)) {
                    LUG_LOG.error("BloomPass::initBlurPass: Can't create blend offscreen image view: {}", result);
                    return false;
                }
            }
        }
    }

    // Create offscreen samplers
    {
        API::Builder::Sampler samplerBuilder(device);

        samplerBuilder.setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setMinFilter(VK_FILTER_LINEAR);
        samplerBuilder.setMagFilter(VK_FILTER_LINEAR);
        samplerBuilder.setMipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR);

        for (uint8_t i = 0; i < frameDataSize; ++i) {
            for (auto& blurPass: _framesData[i].blurPasses) {
                // blur[0] sampler
                {
                    VkResult result{VK_SUCCESS};
                    if (!samplerBuilder.build(blurPass.samplers[0], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't blur[0] create sampler: {}", result);
                        return false;
                    }
                }

                // blur[1] sampler
                {
                    VkResult result{VK_SUCCESS};
                    if (!samplerBuilder.build(blurPass.samplers[1], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur[1] sampler: {}", result);
                        return false;
                    }
                }
            }

            // Blend sampler
            {
                VkResult result{VK_SUCCESS};
                if (!samplerBuilder.build(_framesData[i].blendPass.sampler, &result)) {
                    LUG_LOG.error("BloomPass::initBlurPass: Can't create blend sampler: {}", result);
                    return false;
                }
            }
        }
    }

    // Framebuffers
    {

        for (uint32_t i = 0; i < frameDataSize; i++) {
            for (auto& blurPass: _framesData[i].blurPasses) {
                // blur[0] framebuffer
                {
                    const API::RenderPass* renderPass = _horizontalPipeline.getRenderPass();

                    API::Builder::Framebuffer framebufferBuilder(_renderer.getDevice());
                    framebufferBuilder.setRenderPass(renderPass);
                    framebufferBuilder.addAttachment(&blurPass.imagesViews[0]);
                    framebufferBuilder.setWidth(blurPass.images[0].getExtent().width);
                    framebufferBuilder.setHeight(blurPass.images[0].getExtent().height);

                    VkResult result{VK_SUCCESS};
                    if (!framebufferBuilder.build(blurPass.framebuffers[0], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur[0] framebuffer: {}", result);
                        return false;
                    }
                }

                // blur[1] framebuffer
                {
                    const API::RenderPass* renderPass = _verticalPipeline.getRenderPass();

                    API::Builder::Framebuffer framebufferBuilder(_renderer.getDevice());
                    framebufferBuilder.setRenderPass(renderPass);
                    framebufferBuilder.addAttachment(&blurPass.imagesViews[1]);
                    framebufferBuilder.setWidth(blurPass.images[1].getExtent().width);
                    framebufferBuilder.setHeight(blurPass.images[1].getExtent().height);

                    VkResult result{VK_SUCCESS};
                    if (!framebufferBuilder.build(blurPass.framebuffers[1], &result)) {
                        LUG_LOG.error("BloomPass::initBlurPass: Can't create blur[1] framebuffer: {}", result);
                        return false;
                    }
                }
            }

            // Blend framebuffer
            {
                const API::RenderPass* renderPass = _blendPipeline.getRenderPass();

                API::Builder::Framebuffer framebufferBuilder(_renderer.getDevice());
                framebufferBuilder.setRenderPass(renderPass);
                framebufferBuilder.addAttachment(&_framesData[i].blendPass.imageView);
                framebufferBuilder.setWidth(_framesData[i].blendPass.image.getExtent().width);
                framebufferBuilder.setHeight(_framesData[i].blendPass.image.getExtent().height);

                VkResult result{VK_SUCCESS};
                if (!framebufferBuilder.build(_framesData[i].blendPass.framebuffer, &result)) {
                    LUG_LOG.error("BloomPass::initBlurPass: Can't create blend framebuffer: {}", result);
                    return false;
                }
            }

            // Hdr framebuffer
            {
                const API::RenderPass* renderPass = _hdrPipeline.getRenderPass();

                API::Builder::Framebuffer framebufferBuilder(_renderer.getDevice());
                framebufferBuilder.setRenderPass(renderPass);
                framebufferBuilder.addAttachment(&swapchainImagesViews[i]);
                framebufferBuilder.setWidth(swapchainImages[i].getExtent().width);
                framebufferBuilder.setHeight(swapchainImages[i].getExtent().height);

                VkResult result{VK_SUCCESS};
                if (!framebufferBuilder.build(_framesData[i].hdrFramebuffer, &result)) {
                    LUG_LOG.error("BloomPass::initBlurPass: Can't create hdr framebuffer: {}", result);
                    return false;
                }
            }
        }
    }

    // Create fence for queue submit synchronisation
    API::Fence fence;
    {
        VkResult result{VK_SUCCESS};
        API::Builder::Fence fenceBuilder(device);

        if (!fenceBuilder.build(fence, &result)) {
            LUG_LOG.error("Window::initOffscreenData: Can't create render fence: {}", result);
            return false;
        }
    }

    // Submit queue
    if (!_transferQueue->submit(
        cmdBuffer,
        {},
        {},
        {},
        static_cast<VkFence>(fence)
    )) {
        LUG_LOG.error("BloomPass::initBlurPass Can't submit work to transfer queue");
        return false;
    }

    if (!fence.wait() || !_transferQueue->waitIdle()) {
        LUG_LOG.error("BloomPass::initBlurPass: Can't wait fence");
        return false;
    }

    fence.destroy();

    // Reset the command buffer because we need it in BloomPass::buildEndCommandBuffer
    if (!cmdBuffer.reset()) {
        LUG_LOG.error("BloomPass::initBlurPass: Can't reset command buffer");
        return false;
    }

    return true;
}

bool BloomPass::buildEndCommandBuffer() {
    auto& swapchainImages = _window.getSwapchain().getImages();
    auto& glowOffscreenImages = _window.getGlowOffscreenImages();
    uint32_t frameDataSize = (uint32_t)swapchainImages.size();

    for (uint32_t i = 0; i < frameDataSize; ++i) {

        // transferCmdBuffers[0] generation
        // Copy glow image into blur[0] pass image
        {
            // Build command buffer image color attachment to present
            API::CommandBuffer& cmdBuffer = _framesData[i].transferCmdBuffers[0];

            if (!cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT)) {
                return false;
            }

            // Prepare glow image for copying
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &glowOffscreenImages[i];

                cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            }

            // Clear blur[0] and blend images to black
            {
                VkClearColorValue clearColor;
                std::memset(&clearColor, 0, sizeof(clearColor));

                const VkImageSubresourceRange range{
                    /* range.aspectMask */ VK_IMAGE_ASPECT_COLOR_BIT,
                    /* range.baseMipLevel */ 0,
                    /* range.levelCount */ 1,
                    /* range.baseArrayLayer*/ 0,
                    /* range.layerCount */ 1
                };

                for (auto& blurPass: _framesData[i].blurPasses) {
                    vkCmdClearColorImage(
                        static_cast<VkCommandBuffer>(cmdBuffer),
                        static_cast<VkImage>(blurPass.images[0]),
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        &clearColor,
                        1,
                        &range
                    );
                }

                vkCmdClearColorImage(
                    static_cast<VkCommandBuffer>(cmdBuffer),
                    static_cast<VkImage>(_framesData[i].blendPass.image),
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    &clearColor,
                    1,
                    &range
                );
            }


            // Copy glow image into blur[0] images
            {
                VkImageBlit blitRegion = {};

                blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blitRegion.srcSubresource.baseArrayLayer = 0;
                blitRegion.srcSubresource.mipLevel = 0;
                blitRegion.srcSubresource.layerCount = 1;
                blitRegion.srcOffsets[0] = { 0, 0, 0 };
                blitRegion.srcOffsets[1] = {
                    static_cast<int32_t>(glowOffscreenImages[0].getExtent().width),
                    static_cast<int32_t>(glowOffscreenImages[0].getExtent().height),
                    1
                };

                for (auto& blurPass: _framesData[i].blurPasses) {
                    blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    blitRegion.dstSubresource.baseArrayLayer = 0;
                    blitRegion.dstSubresource.mipLevel = 0;
                    blitRegion.dstSubresource.layerCount = 1;
                    blitRegion.dstOffsets[0] = { 0, 0, 0 };
                    blitRegion.dstOffsets[1] = {
                        static_cast<int32_t>(blurPass.images[0].getExtent().width),
                        static_cast<int32_t>(blurPass.images[0].getExtent().height),
                        1
                    };

                    const API::CommandBuffer::CmdBlitImage cmdBlitImage{
                        /* cmdBlitImage.srcImage      */ glowOffscreenImages[i],
                        /* cmdBlitImage.srcImageLayout  */ VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        /* cmdBlitImage.dstImage      */ blurPass.images[0],
                        /* cmdBlitImage.dsrImageLayout        */ VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        /* cmdBlitImage.regions      */ { blitRegion },
                        /* cmdBlitImage.filter       */ VK_FILTER_LINEAR
                    };

                    cmdBuffer.blitImage(cmdBlitImage);

                    // Prepare blur[0] image for reading in shader
                    {
                        API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                        pipelineBarrier.imageMemoryBarriers.resize(1);
                        pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                        pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                        pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                        pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[0];

                        cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
                    }
                }
            }


            // Prepare blend image for write
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &_framesData[i].blendPass.image;

                cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            }

            if (!cmdBuffer.end()) {
                return false;
            }
        }

        // transferCmdBuffers[1] generation
        // Copy blur[0] image into window image
        {
            // Build command buffer image color attachment to present
            API::CommandBuffer& cmdBuffer = _framesData[i].transferCmdBuffers[1];

            if (!cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT)) {
                return false;
            }

            for (auto& blurPass: _framesData[i].blurPasses) {
                // Prepare blur[0] image for clearing (next frame)
                {
                    API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                    pipelineBarrier.imageMemoryBarriers.resize(1);
                    pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[0];

                    cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
                }

                // Prepare blur[1] image for writing (next frame)
                {
                    API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                    pipelineBarrier.imageMemoryBarriers.resize(1);
                    pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                    pipelineBarrier.imageMemoryBarriers[0].image = &blurPass.images[1];

                    cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
                }
            }


            // Prepare blend image for clearing (next frame)
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &_framesData[i].blendPass.image;

                cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            }

            // Prepare glow image for writing (next frame)
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &glowOffscreenImages[i];

                cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            }

            if (!cmdBuffer.end()) {
                return false;
            }
        }
    }

    return true;
}

} // Render
} // Vulkan
} // Graphics
} // lug
