#include <lug/Graphics/Vulkan/Render/SkyBox.hpp>

#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Image.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Math/Constant.hpp>
#include <lug/Math/Geometry/Transform.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

API::GraphicsPipeline SkyBox::_pipeline;
API::GraphicsPipeline SkyBox::_irradianceMapPipeline;
lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh> SkyBox::_mesh;
uint32_t SkyBox::_skyBoxCount{0};

SkyBox::SkyBox(const std::string& name) : ::lug::Graphics::Render::SkyBox(name) {}

SkyBox::~SkyBox() {
    destroy();
}

void SkyBox::destroy() {
    --_skyBoxCount;
    if (_skyBoxCount == 0) {
        SkyBox::_pipeline.destroy();
        SkyBox::_irradianceMapPipeline.destroy();
        // TODO(nokitoo): destroy SkyBox::_mesh (Can't delete resources in resource manager yet)
    }
}

Resource::SharedPtr<lug::Graphics::Render::SkyBox> SkyBox::createIrradianceMap(lug::Graphics::Renderer& renderer) const {
    constexpr uint32_t irradianceMapSize = 128;

    // Constructor of SkyBox is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::SkyBox(_name + "_irradiance_map")};
    Vulkan::Render::SkyBox* irradianceMap = static_cast<Vulkan::Render::SkyBox*>(resource.get());

    Vulkan::Renderer& vkRenderer = static_cast<Vulkan::Renderer&>(renderer);

    lug::Graphics::Builder::Texture textureBuilder(vkRenderer);

    textureBuilder.setType(lug::Graphics::Builder::Texture::Type::CubeMap);
    textureBuilder.setMagFilter(getTexture()->getMagFilter());
    textureBuilder.setMinFilter(getTexture()->getMinFilter());
    textureBuilder.setMipMapFilter(getTexture()->getMipMapFilter());
    textureBuilder.setWrapS(getTexture()->getWrapS());
    textureBuilder.setWrapT(getTexture()->getWrapT());
    textureBuilder.addLayer(irradianceMapSize, irradianceMapSize);
    textureBuilder.addLayer(irradianceMapSize, irradianceMapSize);
    textureBuilder.addLayer(irradianceMapSize, irradianceMapSize);
    textureBuilder.addLayer(irradianceMapSize, irradianceMapSize);
    textureBuilder.addLayer(irradianceMapSize, irradianceMapSize);
    textureBuilder.addLayer(irradianceMapSize, irradianceMapSize);

    irradianceMap->_texture = textureBuilder.build();
    if (!irradianceMap->_texture) {
        LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't create the irradiance map texture");
        return nullptr;
    }

    // Create Framebuffer for irradiance map generation
    API::CommandPool commandPool;
    API::CommandBuffer cmdBuffer;
    API::Image depthImage;
    API::Image offscreenImage;
    API::ImageView depthImageView;
    API::ImageView offscreenImageView;
    API::DeviceMemory imagesMemory;
    API::Framebuffer framebuffer;
    API::Fence fence;
    API::DescriptorPool descriptorPool;
    API::DescriptorSet descriptorSet;
    const API::Queue* graphicsQueue = nullptr;
    VkResult result{VK_SUCCESS};
    {
        // Graphics queue
        graphicsQueue = vkRenderer.getDevice().getQueue("queue_graphics");
        if (!graphicsQueue) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't find queue with name queue_graphics");
            return nullptr;
        }

        // Command pool
        API::Builder::CommandPool commandPoolBuilder(vkRenderer.getDevice(), *graphicsQueue->getQueueFamily());
        if (!commandPoolBuilder.build(commandPool, &result)) {
            LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create the graphics command pool: {}", result);
            return nullptr;
        }

        // Command buffer
        API::Builder::CommandBuffer commandBufferBuilder(vkRenderer.getDevice(), commandPool);
        commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        // Create the render command buffer
        if (!commandBufferBuilder.build(cmdBuffer, &result)) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create the command buffer: {}", result);
            return nullptr;
        }

        // Descriptor pool
        {
            API::Builder::DescriptorPool descriptorPoolBuilder(vkRenderer.getDevice());

            descriptorPoolBuilder.setFlags(0);

            descriptorPoolBuilder.setMaxSets(42);

            std::vector<VkDescriptorPoolSize> poolSizes{
                {
                    /* poolSize.type            */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    /* poolSize.descriptorCount */ 42
                }
            };
            descriptorPoolBuilder.setPoolSizes(poolSizes);

            if (!descriptorPoolBuilder.build(descriptorPool, &result)) {
                LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create the descriptor pool: {}", result);
                return nullptr;
            }
        }

        // Descriptor set
        {
            API::Builder::DescriptorSet descriptorSetBuilder(vkRenderer.getDevice(), descriptorPool);
            descriptorSetBuilder.setDescriptorSetLayouts({static_cast<VkDescriptorSetLayout>(SkyBox::_irradianceMapPipeline.getLayout()->getDescriptorSetLayouts()[0])});

            if (!descriptorSetBuilder.build(descriptorSet, &result)) {
                LUG_LOG.error("DescriptorSetPool: Can't create descriptor set: {}", result);
                return nullptr;
            }

            auto texture = Resource::SharedPtr<Render::Texture>::cast(getTexture());
            descriptorSet.updateImages(
                0,
                0,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                {
                    {
                        /* sampler       */ static_cast<VkSampler>(texture->getSampler()),
                        /* imageView     */ static_cast<VkImageView>(texture->getImageView()),
                        /* imageLayout   */ VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    }
                }
            );
        }

        // Fence
        {
            API::Builder::Fence fenceBuilder(vkRenderer.getDevice());

            if (!fenceBuilder.build(fence, &result)) {
                LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create render fence: {}", result);
                return nullptr;
            }
        }

        // Depth and offscreen images
        {
            API::Builder::DeviceMemory deviceMemoryBuilder(vkRenderer.getDevice());
            deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            // Create image and add it to API::Builder::DeviceMemory
            const VkExtent3D extent{
                /* extent.width     */ irradianceMapSize,
                /* extent.height    */ irradianceMapSize,
                /* extent.depth     */ 1
            };


            // Create depth buffer image
            {
                API::Builder::Image imageBuilder(vkRenderer.getDevice());

                imageBuilder.setExtent(extent);
                imageBuilder.setUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
                imageBuilder.setPreferedFormats({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT});
                imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

                if (!imageBuilder.build(depthImage, &result)) {
                    LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create depth buffer image: {}", result);
                    return nullptr;
                }

                if (!deviceMemoryBuilder.addImage(depthImage)) {
                    LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't add image to device memory");
                    return nullptr;
                }
            }

            // Create offscreen image
            {
                API::Builder::Image imageBuilder(vkRenderer.getDevice());

                imageBuilder.setExtent(extent);
                imageBuilder.setUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
                imageBuilder.setPreferedFormats({ VK_FORMAT_R32G32B32A32_SFLOAT });
                imageBuilder.setQueueFamilyIndices({ graphicsQueue->getQueueFamily()->getIdx() });
                imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);

                if (!imageBuilder.build(offscreenImage, &result)) {
                    LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create depth buffer image: {}", result);
                    return nullptr;
                }

                if (!deviceMemoryBuilder.addImage(offscreenImage)) {
                    LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't add image to device memory");
                    return nullptr;
                }
            }

            // Initialize depth buffer memory
            {
                if (!deviceMemoryBuilder.build(imagesMemory, &result)) {
                    LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create device memory: {}", result);
                    return nullptr;
                }
            }

            // Create depth buffer image view
            {
                API::Builder::ImageView imageViewBuilder(vkRenderer.getDevice(), depthImage);

                imageViewBuilder.setFormat(depthImage.getFormat());
                imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT);

                if (!imageViewBuilder.build(depthImageView, &result)) {
                    LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create depth buffer image view: {}", result);
                    return nullptr;
                }
            }

            // Create offscreen image view
            {
                API::Builder::ImageView imageViewBuilder(vkRenderer.getDevice(), offscreenImage);

                imageViewBuilder.setFormat(offscreenImage.getFormat());
                imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT);

                if (!imageViewBuilder.build(offscreenImageView, &result)) {
                    LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't create depth buffer image view: {}", result);
                    return nullptr;
                }
            }
        }

        // Create framebuffer
        {
            // Create depth buffer image view
            API::Builder::Framebuffer framebufferBuilder(vkRenderer.getDevice());

            const API::RenderPass* renderPass = SkyBox::_irradianceMapPipeline.getRenderPass();

            framebufferBuilder.setRenderPass(renderPass);
            framebufferBuilder.addAttachment(&offscreenImageView);
            framebufferBuilder.addAttachment(&depthImageView);
            framebufferBuilder.setWidth(irradianceMapSize);
            framebufferBuilder.setHeight(irradianceMapSize);

            if (!framebufferBuilder.build(framebuffer, &result)) {
                LUG_LOG.error("Forward::initFramebuffers: Can't create framebuffer: {}", result);
                return nullptr;
            }
        }
    }

    // Generate the irradiance cube map
    {
        if (!cmdBuffer.begin()) {
            return nullptr;
        }

        cmdBuffer.bindPipeline(SkyBox::_irradianceMapPipeline);

        // Bind descriptor set of the skybox
        {
            const API::CommandBuffer::CmdBindDescriptors skyBoxBind{
                /* skyBoxBind.pipelineLayout     */ *SkyBox::_irradianceMapPipeline.getLayout(),
                /* skyBoxBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                /* skyBoxBind.firstSet           */ 0,
                /* skyBoxBind.descriptorSets     */ {&descriptorSet},
                /* skyBoxBind.dynamicOffsets     */ {},
            };

            cmdBuffer.bindDescriptorSets(skyBoxBind);
        }

        auto& primitiveSet = SkyBox::getMesh()->getPrimitiveSets()[0];

        cmdBuffer.bindVertexBuffers(
            {static_cast<API::Buffer*>(primitiveSet.position->_data)},
            {0}
        );

        API::Buffer* indicesBuffer = static_cast<API::Buffer*>(primitiveSet.indices->_data);
        cmdBuffer.bindIndexBuffer(*indicesBuffer, VK_INDEX_TYPE_UINT16);
        const API::CommandBuffer::CmdDrawIndexed cmdDrawIndexed {
            /* cmdDrawIndexed.indexCount    */ primitiveSet.indices->buffer.elementsCount,
            /* cmdDrawIndexed.instanceCount */ 1,
        };

        Math::Mat4x4f projection = Math::Geometry::perspective(Math::halfPi<float>(), 1.0f, 0.1f, static_cast<float>(irradianceMapSize));
        std::vector<Math::Mat4x4f> matrices = {
            Math::Geometry::lookAt<float>({0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}),
            Math::Geometry::lookAt<float>({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}),
            Math::Geometry::lookAt<float>({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}),
            Math::Geometry::lookAt<float>({0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}),
            Math::Geometry::lookAt<float>({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}),
            Math::Geometry::lookAt<float>({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f})
        };

        // Change offscreen image layout to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL for render
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &offscreenImage;

            cmdBuffer.pipelineBarrier(pipelineBarrier);
        }

        // Change irradiance layout to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL for copy
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &Resource::SharedPtr<Render::Texture>::cast(irradianceMap->_texture)->getImage();
            pipelineBarrier.imageMemoryBarriers[0].subresourceRange.layerCount = 6;

            cmdBuffer.pipelineBarrier(pipelineBarrier);
        }


        for (uint32_t i = 0; i < 6; ++i) {
            // Begin of the render pass
            {
                // All the pipelines have the same renderPass
                const API::RenderPass* renderPass = SkyBox::_irradianceMapPipeline.getRenderPass();

                API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
                    /* beginRenderPass.framebuffer  */ framebuffer,
                    /* beginRenderPass.renderArea   */ {},
                    /* beginRenderPass.clearValues  */ {}
                };

                beginRenderPass.renderArea.offset = {0, 0};
                beginRenderPass.renderArea.extent = {irradianceMapSize, irradianceMapSize};

                beginRenderPass.clearValues.resize(2);
                beginRenderPass.clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
                beginRenderPass.clearValues[1].depthStencil = {1.0f, 0};

                cmdBuffer.beginRenderPass(*renderPass, beginRenderPass);

                const VkViewport vkViewport{
                    /* vkViewport.x         */ 0.0f,
                    /* vkViewport.y         */ 0.0f,
                    /* vkViewport.width     */ static_cast<float>(irradianceMapSize),
                    /* vkViewport.height    */ static_cast<float>(irradianceMapSize),
                    /* vkViewport.minDepth  */ 0.0f,
                    /* vkViewport.maxDepth  */ 1.0f,
                };

                const VkRect2D scissor{
                    /* scissor.offset */ {
                        0,
                        0
                    },
                    /* scissor.extent */ {
                       static_cast<uint32_t>(irradianceMapSize),
                       static_cast<uint32_t>(irradianceMapSize)
                    }
                };

                cmdBuffer.setViewport({vkViewport});
                cmdBuffer.setScissor({scissor});
            }

            const Math::Mat4x4f pushConstants[] = {
                projection * matrices[i]
            };

            const API::CommandBuffer::CmdPushConstants cmdPushConstants{
                /* cmdPushConstants.layout      */ static_cast<VkPipelineLayout>(*SkyBox::_irradianceMapPipeline.getLayout()),
                /* cmdPushConstants.stageFlags  */ VK_SHADER_STAGE_VERTEX_BIT,
                /* cmdPushConstants.offset      */ 0,
                /* cmdPushConstants.size        */ sizeof(pushConstants),
                /* cmdPushConstants.values      */ pushConstants
            };

            cmdBuffer.pushConstants(cmdPushConstants);

            cmdBuffer.drawIndexed(cmdDrawIndexed);

            // End of the render pass
            cmdBuffer.endRenderPass();

            // Copy region for transfer from framebuffer to cube face
            VkImageCopy copyRegion = {};

            copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.srcSubresource.baseArrayLayer = 0;
            copyRegion.srcSubresource.mipLevel = 0;
            copyRegion.srcSubresource.layerCount = 1;
            copyRegion.srcOffset = { 0, 0, 0 };

            copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.dstSubresource.baseArrayLayer = i;
            copyRegion.dstSubresource.mipLevel = 0;
            copyRegion.dstSubresource.layerCount = 1;
            copyRegion.dstOffset = { 0, 0, 0 };

            copyRegion.extent.width = irradianceMapSize;
            copyRegion.extent.height = irradianceMapSize;
            copyRegion.extent.depth = 1;

            const API::CommandBuffer::CmdCopyImage cmdCopyImage{
                /* cmdCopyImage.srcImage      */ offscreenImage,
                /* cmdCopyImage.srcImageLayout  */ VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                /* cmdCopyImage.dstImage      */ Resource::SharedPtr<Render::Texture>::cast(irradianceMap->_texture)->getImage(),
                /* cmdCopyImage.dsrImageLayout        */ VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                /* cmdCopyImage.regions      */ { copyRegion }
            };

            cmdBuffer.copyImage(cmdCopyImage);

            // Change offscreen image layout to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL for render
            {
                API::CommandBuffer::CmdPipelineBarrier::ImageMemoryBarrier imageMemoryBarrier;
                imageMemoryBarrier.image = &offscreenImage;
                imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                API::CommandBuffer::CmdPipelineBarrier cmdPipelineBarrier;
                cmdPipelineBarrier.imageMemoryBarriers.push_back(std::move(imageMemoryBarrier));
                cmdBuffer.pipelineBarrier(cmdPipelineBarrier);
            }
        }

        if (!cmdBuffer.end()) {
            return nullptr;
        }
    }

    if (!graphicsQueue->submit(
        cmdBuffer,
        {},
        {},
        {},
        static_cast<VkFence>(fence)
    )) {
        LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap: Can't submit work to graphics queue: {}", result);
        return nullptr;
    }

    if (!fence.wait() || !graphicsQueue->waitIdle()) {
        LUG_LOG.error("Forward::iniResource::SharedPtr<::lug::Graphics::Render::SkyBox>::createIrradianceMap:: Can't wait fence");
        return nullptr;
    }

    cmdBuffer.destroy();
    commandPool.destroy();
    depthImage.destroy();
    offscreenImage.destroy();
    depthImageView.destroy();
    offscreenImageView.destroy();
    framebuffer.destroy();
    descriptorPool.destroy();
    fence.destroy();

    return vkRenderer.getResourceManager()->add<::lug::Graphics::Render::SkyBox>(std::move(resource));
}

} // Render
} // Vulkan
} // Graphics
} // lug
