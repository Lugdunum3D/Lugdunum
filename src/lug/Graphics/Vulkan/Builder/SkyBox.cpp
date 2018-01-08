#include <lug/Graphics/Vulkan/Builder/SkyBox.hpp>

#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Builder/SkyBox.hpp>
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/SkyBox.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {
namespace SkyBox {

static bool initMesh(Renderer& renderer, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh>& skyBoxMesh) {
    const std::vector<lug::Math::Vec3f> positions = {
        // Back
        {-1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {-1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, -1.0f},

        // Front
        {-1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},

        // Left
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, 1.0f},
        {-1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, 1.0f},

        // Right
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, 1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, 1.0f},

        // Bottom
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, 1.0f},

        // Top
        {-1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, 1.0}
    };

    const std::vector<uint16_t> indices = {
        // Back
        0, 2, 1,
        1, 2, 3,

        // Front
        6, 4, 5,
        7, 6, 5,

        // Left
        10, 8, 9,
        11, 10, 9,

        // Right
        14, 13, 12,
        15, 13, 14,

        // Bottom
        17, 16, 19,
        19, 16, 18,

        // Top
        23, 20, 21,
        22, 20, 23
    };

    // Build the mesh
    {
        lug::Graphics::Builder::Mesh meshBuilder(renderer);
        meshBuilder.setName("skybox");

        lug::Graphics::Builder::Mesh::PrimitiveSet* primitiveSet = meshBuilder.addPrimitiveSet();

        primitiveSet->setMode(lug::Graphics::Render::Mesh::PrimitiveSet::Mode::Triangles);

        primitiveSet->addAttributeBuffer(
            indices.data(),
            sizeof(uint16_t),
            static_cast<uint32_t>(indices.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice
        );

        primitiveSet->addAttributeBuffer(
            positions.data(),
            sizeof(lug::Math::Vec3f),
            static_cast<uint32_t>(positions.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Position
        );

        skyBoxMesh = meshBuilder.build();

        if (!skyBoxMesh) {
            LUG_LOG.error("initMesh: Can't create the skyBox mesh");
            return false;
        }
    }

    return true;
}

static bool initBrdfLut(Renderer& renderer, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture>& brdfLut) {
    constexpr uint32_t brdfLutSize = 512;

    Vulkan::Renderer& vkRenderer = static_cast<Vulkan::Renderer&>(renderer);
    auto brdfLutPipeline = vkRenderer.getPipeline(Render::Pipeline::getBrdfLutBaseId());

    if (!brdfLutPipeline) {
        LUG_LOG.error("Skybox::initBrdfLut: Can't get the brdf lut pipeline");
        return false;
    }

    // Create the texture
    // TODO: Set sampler borderColor to VK_BORDER_COOLOR_FLOAT_OPAQUE_WHITE
    lug::Graphics::Builder::Texture textureBuilder(renderer);

    textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);
    textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);

    if (!textureBuilder.addLayer(brdfLutSize, brdfLutSize, lug::Graphics::Render::Texture::Format::R16G16_SFLOAT)) {
        LUG_LOG.error("Skybox::initBrdfLut: Can't create the brdf lut texture layer");
        return false;
    }

    brdfLut = textureBuilder.build();
    if (!brdfLut) {
        LUG_LOG.error("Skybox::initBrdfLut: Can't create the brdf lut texture");
        return false;
    }

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture> vKBrdfLut = Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(brdfLut);

    // Create Framebuffer for brdf lut generation
    API::CommandPool commandPool;
    API::CommandBuffer cmdBuffer;
    API::Framebuffer framebuffer;
    API::Fence fence;
    const API::Queue* graphicsQueue = nullptr;
    VkResult result{VK_SUCCESS};
    {
        // Graphics queue
        graphicsQueue = vkRenderer.getDevice().getQueue("queue_graphics");
        if (!graphicsQueue) {
            LUG_LOG.error("Skybox::initBrdfLut: Can't find queue with name queue_graphics");
            return false;
        }

        // Command pool
        API::Builder::CommandPool commandPoolBuilder(vkRenderer.getDevice(), *graphicsQueue->getQueueFamily());
        if (!commandPoolBuilder.build(commandPool, &result)) {
            LUG_LOG.error("Skybox::initBrdfLut: Can't create the graphics command pool: {}", result);
            return false;
        }

        // Command buffer
        API::Builder::CommandBuffer commandBufferBuilder(vkRenderer.getDevice(), commandPool);
        commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        // Create the render command buffer
        if (!commandBufferBuilder.build(cmdBuffer, &result)) {
            LUG_LOG.error("Skybox::initBrdfLut: Can't create the command buffer: {}", result);
            return false;
        }

        // Fence
        {
            API::Builder::Fence fenceBuilder(vkRenderer.getDevice());

            if (!fenceBuilder.build(fence, &result)) {
                LUG_LOG.error("Skybox::initBrdfLut: Can't create render fence: {}", result);
                return false;
            }
        }

        // Create framebuffer
        {
            API::Builder::Framebuffer framebufferBuilder(vkRenderer.getDevice());

            const API::RenderPass* renderPass = brdfLutPipeline->getPipelineAPI().getRenderPass();

            framebufferBuilder.setRenderPass(renderPass);
            framebufferBuilder.addAttachment(&vKBrdfLut->getImageView());
            framebufferBuilder.setWidth(brdfLutSize);
            framebufferBuilder.setHeight(brdfLutSize);

            if (!framebufferBuilder.build(framebuffer, &result)) {
                LUG_LOG.error("tFramebuffers: Can't create framebuffer: {}", result);
                return false;
            }
        }
    }

    // Generate the brdf lut
    {
        if (!cmdBuffer.begin()) {
            return false;
        }

        cmdBuffer.bindPipeline(brdfLutPipeline->getPipelineAPI());

        // Change texture image layout to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL for render
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &vKBrdfLut->getImage();
            pipelineBarrier.imageMemoryBarriers[0].subresourceRange.layerCount = 1;

            cmdBuffer.pipelineBarrier(pipelineBarrier);
        }

        // Begin of the render pass
        {
            // All the pipelines have the same renderPass
            const API::RenderPass* renderPass = brdfLutPipeline->getPipelineAPI().getRenderPass();

            API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
                /* beginRenderPass.framebuffer  */ framebuffer,
                /* beginRenderPass.renderArea   */ {},
                /* beginRenderPass.clearValues  */ {}
            };

            beginRenderPass.renderArea.offset = {0, 0};
            beginRenderPass.renderArea.extent = {brdfLutSize, brdfLutSize};

            beginRenderPass.clearValues.resize(2);
            beginRenderPass.clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            beginRenderPass.clearValues[1].depthStencil = {1.0f, 0};

            cmdBuffer.beginRenderPass(*renderPass, beginRenderPass);

            const VkViewport vkViewport{
                /* vkViewport.x         */ 0.0f,
                /* vkViewport.y         */ 0.0f,
                /* vkViewport.width     */ static_cast<float>(brdfLutSize),
                /* vkViewport.height    */ static_cast<float>(brdfLutSize),
                /* vkViewport.minDepth  */ 0.0f,
                /* vkViewport.maxDepth  */ 1.0f,
            };

            const VkRect2D scissor{
                /* scissor.offset */ {
                    0,
                    0
                },
                /* scissor.extent */ {
                   static_cast<uint32_t>(brdfLutSize),
                   static_cast<uint32_t>(brdfLutSize)
                }
            };

            cmdBuffer.setViewport({vkViewport});
            cmdBuffer.setScissor({scissor});
        }

        cmdBuffer.draw({
            /* vertexCount */ 3,
            /* instanceCount */ 1,
            /* firstVertex */ 0,
            /* firstInstance */ 0
        });

        // End of the render pass
        cmdBuffer.endRenderPass();

        // Change texture layout to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &vKBrdfLut->getImage();
            pipelineBarrier.imageMemoryBarriers[0].subresourceRange.layerCount = 1;

            cmdBuffer.pipelineBarrier(pipelineBarrier);
        }

        if (!cmdBuffer.end()) {
            return false;
        }
    }

    if (!graphicsQueue->submit(
        cmdBuffer,
        {},
        {},
        {},
        static_cast<VkFence>(fence)
    )) {
        LUG_LOG.error("Skybox::initBrdfLut: Can't submit work to graphics queue: {}", result);
        return false;
    }

    if (!fence.wait() || !graphicsQueue->waitIdle()) {
        LUG_LOG.error("Skybox::initBrdfLut:: Can't wait fence");
        return false;
    }

    cmdBuffer.destroy();
    commandPool.destroy();
    framebuffer.destroy();
    fence.destroy();

    return true;
}

Resource::SharedPtr<::lug::Graphics::Render::SkyBox> build(const ::lug::Graphics::Builder::SkyBox& builder) {
    // Constructor of SkyBox is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::SkyBox(builder._name)};
    Vulkan::Render::SkyBox* skyBox = static_cast<Vulkan::Render::SkyBox*>(resource.get());

    Vulkan::Renderer& renderer = static_cast<Vulkan::Renderer&>(builder._renderer);

    // Load background image
    if (builder._backgroundFilename.size())
    {
        lug::Graphics::Builder::Texture textureBuilder(builder._renderer);

        textureBuilder.setMagFilter(builder._magFilter);
        textureBuilder.setMinFilter(builder._minFilter);
        textureBuilder.setMipMapFilter(builder._mipMapFilter);
        textureBuilder.setWrapS(builder._wrapS);
        textureBuilder.setWrapT(builder._wrapT);

        if (!textureBuilder.addLayer(builder._backgroundFilename)) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't create skybox layers");
            return nullptr;
        }

        skyBox->_backgroundTexture = textureBuilder.build();
        if (!skyBox->_backgroundTexture) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't create skyBox texture");
            return nullptr;
        }
    }

    // Load environnement image
    if (builder._environnementFilename.size())
    {
        lug::Graphics::Builder::Texture textureBuilder(builder._renderer);

        textureBuilder.setMagFilter(builder._magFilter);
        textureBuilder.setMinFilter(builder._minFilter);
        textureBuilder.setMipMapFilter(builder._mipMapFilter);
        textureBuilder.setWrapS(builder._wrapS);
        textureBuilder.setWrapT(builder._wrapT);

        if (!textureBuilder.addLayer(builder._environnementFilename, true)) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't create skybox layers");
            return nullptr;
        }

        skyBox->_environnementTexture = textureBuilder.build();
        if (!skyBox->_environnementTexture) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't create skyBox texture");
            return nullptr;
        }
    }

    // Init the skyBox pipeline and mesh only one time
    ++Render::SkyBox::_skyBoxCount;
    if (Render::SkyBox::_skyBoxCount == 1) {
        if (!initBrdfLut(renderer, Render::SkyBox::_brdfLut) ||
            !initMesh(renderer, Render::SkyBox::_mesh)) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't init skybox pipeline/mesh resources");
            return nullptr;
        }
    }

    return builder._renderer.getResourceManager()->add<::lug::Graphics::Render::SkyBox>(std::move(resource));
}

} // SkyBox
} // Builder
} // Vulkan
} // Graphics
} // lug
