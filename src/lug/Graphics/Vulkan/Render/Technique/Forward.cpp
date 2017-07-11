#include <lug/Graphics/Vulkan/Render/Technique/Forward.hpp>

#include <algorithm>

#include <lug/Config.hpp>
#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Image.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Render/Material.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/Render/Queue.hpp>
#include <lug/Graphics/Vulkan/Render/View.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Math/Geometry/Transform.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace Technique {

std::unique_ptr<BufferPool::Camera> Forward::_cameraBufferPool = nullptr;
std::unique_ptr<BufferPool::Light> Forward::_lightBufferPool = nullptr;
std::unique_ptr<BufferPool::Material> Forward::_materialBufferPool = nullptr;

std::unique_ptr<DescriptorSetPool::Camera> Forward::_cameraDescriptorSetPool = nullptr;
std::unique_ptr<DescriptorSetPool::Light> Forward::_lightDescriptorSetPool = nullptr;
std::unique_ptr<DescriptorSetPool::Material> Forward::_materialDescriptorSetPool = nullptr;
std::unique_ptr<DescriptorSetPool::MaterialTextures> Forward::_materialTexturesDescriptorSetPool = nullptr;

Forward::Forward(Renderer& renderer, const Render::View& renderView) : Technique(renderer, renderView) {}

bool Forward::render(
    const Render::Queue& renderQueue,
    const API::Semaphore& imageReadySemaphore,
    const API::Semaphore& drawCompleteSemaphore,
    uint32_t currentImageIndex
) {
    FrameData& frameData = _framesData[currentImageIndex];

    frameData.transferFence.wait();
    frameData.transferFence.reset();

    if (!frameData.transferCmdBuffer.reset() || !frameData.transferCmdBuffer.begin()) {
        return false;
    }

    // Get the new (or old) camera buffer
    {
        const BufferPool::SubBuffer* cameraBuffer = _cameraBufferPool->allocate(frameData.transferCmdBuffer, *_renderView.getCamera());
        _cameraBufferPool->free(frameData.cameraBuffer);
        frameData.cameraBuffer = cameraBuffer;
    }

    frameData.renderFence.wait();
    frameData.renderFence.reset();

    if (!frameData.renderCmdBuffer.reset() || !frameData.renderCmdBuffer.begin()) {
        return false;
    }

    // Begin of the render pass
    {
        // All the pipelines have the same renderPass
        const API::RenderPass* renderPass = _renderer.getPipeline(Pipeline::getBaseId())->getPipelineAPI().getRenderPass();

        API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
            /* beginRenderPass.framebuffer  */ frameData.framebuffer,
            /* beginRenderPass.renderArea   */ {},
            /* beginRenderPass.clearValues  */ {}
        };

        const auto& viewport = _renderView.getViewport();
        beginRenderPass.renderArea.offset = {static_cast<int32_t>(viewport.offset.x), static_cast<int32_t>(viewport.offset.y)};
        beginRenderPass.renderArea.extent = {static_cast<uint32_t>(viewport.extent.width), static_cast<uint32_t>(viewport.extent.height)};

        beginRenderPass.clearValues.resize(2);
        beginRenderPass.clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        beginRenderPass.clearValues[1].depthStencil = {1.0f, 0};

        frameData.renderCmdBuffer.beginRenderPass(*renderPass, beginRenderPass);

        const VkViewport vkViewport{
            /* vkViewport.x         */ viewport.offset.x,
            /* vkViewport.y         */ viewport.offset.y,
            /* vkViewport.width     */ viewport.extent.width,
            /* vkViewport.height    */ viewport.extent.height,
            /* vkViewport.minDepth  */ viewport.minDepth,
            /* vkViewport.maxDepth  */ viewport.maxDepth,
        };

        const VkRect2D scissor{
            /* scissor.offset */ {
               static_cast<int32_t>(_renderView.getScissor().offset.x),
               static_cast<int32_t>(_renderView.getScissor().offset.y)
            },
            /* scissor.extent */ {
               static_cast<uint32_t>(_renderView.getScissor().extent.width),
               static_cast<uint32_t>(_renderView.getScissor().extent.height)
            }
        };

        frameData.renderCmdBuffer.setViewport({vkViewport});
        frameData.renderCmdBuffer.setScissor({scissor});
    }

    // Get the new (or old) camera descriptor set
    {
        const DescriptorSetPool::DescriptorSet* cameraDescriptorSet = _cameraDescriptorSetPool->allocate(*frameData.cameraBuffer);
        _cameraDescriptorSetPool->free(frameData.cameraDescriptorSet);
        frameData.cameraDescriptorSet = cameraDescriptorSet;
    }

    // Bind descriptor set of the camera
    {
        const API::CommandBuffer::CmdBindDescriptors cameraBind{
            /* cameraBind.pipelineLayout    */ *_renderer.getPipeline(Pipeline::getBaseId())->getPipelineAPI().getLayout(),
            /* cameraBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* cameraBind.firstSet           */ 0,
            /* cameraBind.descriptorSets     */ {&frameData.cameraDescriptorSet->getDescriptorSet()},
            /* cameraBind.dynamicOffsets     */ {frameData.cameraBuffer->getOffset()},
        };

        frameData.renderCmdBuffer.bindDescriptorSets(cameraBind);
    }

    // Temporary array of light and material buffers use to render this frame
    // they will replace frameData.lightBuffers and frameData.materialBuffers atfer the rendering
    std::vector<const BufferPool::SubBuffer*> lightBuffers;
    std::vector<const BufferPool::SubBuffer*> materialBuffers;

    // Temporary array of light and material descriptor sets use to render this frame
    // they will replace frameData.lightDescriptorSets and frameData.materialDescriptorSets atfer the rendering
    std::vector<const DescriptorSetPool::DescriptorSet*> lightDescriptorSets;
    std::vector<const DescriptorSetPool::DescriptorSet*> materialDescriptorSets;
    std::vector<const DescriptorSetPool::DescriptorSet*> materialTexturesDescriptorSets;

    // Render objects
    {
        // Blend constants are used as dst blend factor
        // We set them to 0 so that there is no blending
        {
            const float blendConstants[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            frameData.renderCmdBuffer.setBlendConstants(blendConstants);
        }

        auto& lights = renderQueue.getLights();
        for (uint32_t i = 0; i < renderQueue.getLightsCount(); i += 50) {
            // Get the new (or old) light buffer
            const BufferPool::SubBuffer* lightBuffer = _lightBufferPool->allocate(
                currentImageIndex,
                frameData.transferCmdBuffer,
                {lights.begin() + i, i + 50 > renderQueue.getLightsCount() ? lights.begin() + renderQueue.getLightsCount() : lights.begin() + i + 50}
            );
            lightBuffers.push_back(lightBuffer);

            // Get the new (or old) light descriptor set
            const DescriptorSetPool::DescriptorSet* lightDescriptorSet = _lightDescriptorSetPool->allocate(*lightBuffer);

            // Bind descriptor set of the light
            {
                const API::CommandBuffer::CmdBindDescriptors lightBind{
                    /* lightBind.pipelineLayout     */ *_renderer.getPipeline(Pipeline::getBaseId())->getPipelineAPI().getLayout(),
                    /* lightBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                    /* lightBind.firstSet           */ 1,
                    /* lightBind.descriptorSets     */ {&lightDescriptorSet->getDescriptorSet()},
                    /* lightBind.dynamicOffsets     */ {lightBuffer->getOffset()},
                };

                frameData.renderCmdBuffer.bindDescriptorSets(lightBind);
            }

            // After the first batch of lights we need to re-enable the blend
            if (i == 50) {
                const float blendConstants[4] = {1.0f, 1.0f, 1.0f, 1.0f};
                frameData.renderCmdBuffer.setBlendConstants(blendConstants);
            }

            for (const auto it : renderQueue.getPrimitiveSets()) {
                // Bind pipeline
                Resource::SharedPtr<Render::Pipeline> pipeline = _renderer.getPipeline(it.first);
                frameData.renderCmdBuffer.bindPipeline(pipeline->getPipelineAPI());

                // Display primitive set by primitive set
                for (const auto primitiveSetInstance : it.second) {
                    auto& node = *primitiveSetInstance.node;
                    const auto& primitiveSet = *primitiveSetInstance.primitiveSet;
                    auto& material = *primitiveSetInstance.material;

                    const Math::Mat4x4f pushConstants[] = {
                        node.getTransform()
                    };

                    const API::CommandBuffer::CmdPushConstants cmdPushConstants{
                        /* cmdPushConstants.layout      */ static_cast<VkPipelineLayout>(*pipeline->getPipelineAPI().getLayout()),
                        /* cmdPushConstants.stageFlags  */ VK_SHADER_STAGE_VERTEX_BIT,
                        /* cmdPushConstants.offset      */ 0,
                        /* cmdPushConstants.size        */ sizeof(pushConstants),
                        /* cmdPushConstants.values      */ pushConstants
                    };
                    frameData.renderCmdBuffer.pushConstants(cmdPushConstants);

                    // Get the new (or old) material buffer
                    const BufferPool::SubBuffer* materialBuffer = _materialBufferPool->allocate(frameData.transferCmdBuffer, material);
                    materialBuffers.push_back(materialBuffer);

                    // Get the new (or old) material descriptor set
                    const DescriptorSetPool::DescriptorSet* materialDescriptorSet = _materialDescriptorSetPool->allocate(*materialBuffer);
                    materialDescriptorSets.push_back(materialDescriptorSet);

                    std::vector<const API::DescriptorSet*> materialDescriptorSetsBind{&materialDescriptorSet->getDescriptorSet()};

                    if (pipeline->getPipelineAPI().getLayout()->getDescriptorSetLayouts().size() > 3) {
                        // Get the new (or old) material descriptor set
                        const DescriptorSetPool::DescriptorSet* materialTexturesDescriptorSet = _materialTexturesDescriptorSetPool->allocate(
                            pipeline->getPipelineAPI(),
                            [&material]() {
                                std::vector<const ::lug::Graphics::Vulkan::Render::Texture*> textures;

                                if (material.getBaseColorTexture().texture) {
                                    textures.push_back(static_cast<const ::lug::Graphics::Vulkan::Render::Texture*>(material.getBaseColorTexture().texture.get()));
                                }

                                if (material.getMetallicRoughnessTexture().texture) {
                                    textures.push_back(static_cast<const ::lug::Graphics::Vulkan::Render::Texture*>(material.getMetallicRoughnessTexture().texture.get()));
                                }

                                if (material.getNormalTexture().texture) {
                                    textures.push_back(static_cast<const ::lug::Graphics::Vulkan::Render::Texture*>(material.getNormalTexture().texture.get()));
                                }

                                if (material.getOcclusionTexture().texture) {
                                    textures.push_back(static_cast<const ::lug::Graphics::Vulkan::Render::Texture*>(material.getOcclusionTexture().texture.get()));
                                }

                                if (material.getEmissiveTexture().texture) {
                                    textures.push_back(static_cast<const ::lug::Graphics::Vulkan::Render::Texture*>(material.getEmissiveTexture().texture.get()));
                                }


                                return textures;
                            }()
                        );
                        materialTexturesDescriptorSets.push_back(materialTexturesDescriptorSet);
                        materialDescriptorSetsBind.push_back(&materialTexturesDescriptorSet->getDescriptorSet());
                    }

                    // Bind descriptor set of the material
                    {
                        const API::CommandBuffer::CmdBindDescriptors materialBind{
                            /* materialBind.pipelineLayout     */ *pipeline->getPipelineAPI().getLayout(),
                            /* materialBind.pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                            /* materialBind.firstSet           */ 2,
                            /* materialBind.descriptorSets     */ materialDescriptorSetsBind,
                            /* materialBind.dynamicOffsets     */ {materialBuffer->getOffset()},
                        };

                        frameData.renderCmdBuffer.bindDescriptorSets(materialBind);
                    }

                    if (!primitiveSet.position || !primitiveSet.normal) {
                        LUG_LOG.warn("Forward::render: Mesh should have positions and normals data");
                        continue;
                    }

                    std::vector<const API::Buffer*> vertexBuffers{
                        static_cast<API::Buffer*>(primitiveSet.position->_data),
                        static_cast<API::Buffer*>(primitiveSet.normal->_data)
                    };

                    if (primitiveSet.tangent) {
                        vertexBuffers.push_back(static_cast<API::Buffer*>(primitiveSet.tangent->_data));
                    }

                    for (const auto& texCoord: primitiveSet.texCoords) {
                        vertexBuffers.push_back(static_cast<API::Buffer*>(texCoord->_data));
                    }

                    for (const auto& color: primitiveSet.colors) {
                        vertexBuffers.push_back(static_cast<API::Buffer*>(color->_data));
                    }

                    const std::vector<VkDeviceSize> offsets(vertexBuffers.size());
                    frameData.renderCmdBuffer.bindVertexBuffers(vertexBuffers, offsets);

                    if (primitiveSet.indices) {
                        API::Buffer* indicesBuffer = static_cast<API::Buffer*>(primitiveSet.indices->_data);
                        frameData.renderCmdBuffer.bindIndexBuffer(*indicesBuffer, VK_INDEX_TYPE_UINT16);
                        const API::CommandBuffer::CmdDrawIndexed cmdDrawIndexed {
                            /* cmdDrawIndexed.indexCount    */ primitiveSet.indices->buffer.elementsCount,
                            /* cmdDrawIndexed.instanceCount */ 1,
                        };

                        frameData.renderCmdBuffer.drawIndexed(cmdDrawIndexed);
                    } else {
                        const API::CommandBuffer::CmdDraw cmdDraw {
                            /* cmdDrawIndexed.vertexCount   */ primitiveSet.position->buffer.elementsCount,
                            /* cmdDrawIndexed.instanceCount */ 1,
                        };

                        frameData.renderCmdBuffer.draw(cmdDraw);
                    }
                }
            }
        }
    }

    // Free and replace previous lightBuffers
    {
        for (const auto& subBuffer : frameData.lightBuffers) {
            _lightBufferPool->free(subBuffer);
        }

        frameData.lightBuffers = lightBuffers;
    }

    // Free and replace previous materialBuffers
    {
        for (const auto& subBuffer : frameData.materialBuffers) {
            _lightBufferPool->free(subBuffer);
        }

        frameData.materialBuffers = materialBuffers;
    }

    // Free and replace previous lightDescriptorSets
    {
        for (const auto& descriptorSet : frameData.lightDescriptorSets) {
            _lightDescriptorSetPool->free(descriptorSet);
        }

        frameData.lightDescriptorSets = lightDescriptorSets;
    }

    // Free and replace previous materialDescriptorSets
    {
        for (const auto& descriptorSet : frameData.materialDescriptorSets) {
            _materialDescriptorSetPool->free(descriptorSet);
        }

        frameData.materialDescriptorSets = materialDescriptorSets;
    }

    // Free and replace previous materialTexturesDescriptorSets
    {
        for (const auto& descriptorSet : frameData.materialTexturesDescriptorSets) {
            _materialTexturesDescriptorSetPool->free(descriptorSet);
        }

        frameData.materialTexturesDescriptorSets = materialTexturesDescriptorSets;
    }

    // End of the render pass
    frameData.renderCmdBuffer.endRenderPass();

    if (!frameData.renderCmdBuffer.end() || !frameData.transferCmdBuffer.end()) {
        return false;
    }

    return _transferQueue->submit(
        frameData.transferCmdBuffer,
        {static_cast<VkSemaphore>(frameData.transferSemaphore)},
        {},
        {},
        static_cast<VkFence>(frameData.transferFence)
    ) && _graphicsQueue->submit(
        frameData.renderCmdBuffer,
        {static_cast<VkSemaphore>(drawCompleteSemaphore)},
        {static_cast<VkSemaphore>(frameData.transferSemaphore), static_cast<VkSemaphore>(imageReadySemaphore)},
        {VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
        static_cast<VkFence>(frameData.renderFence)
    );
}

bool Forward::init(const std::vector<API::ImageView>& imageViews) {
    VkResult result{VK_SUCCESS};

    // Init graphics queue
    {
        _graphicsQueue = _renderer.getDevice().getQueue("queue_graphics");
        if (!_graphicsQueue) {
            LUG_LOG.error("Forward::init: Can't find queue with name queue_graphics");
            return false;
        }

        API::Builder::CommandPool commandPoolBuilder(_renderer.getDevice(), *_graphicsQueue->getQueueFamily());
        if (!commandPoolBuilder.build(_graphicsCommandPool, &result)) {
            LUG_LOG.error("Forward::init: Can't create the graphics command pool: {}", result);
            return false;
        }
    }

    // Init transfer queue
    {
        _transferQueue = _renderer.getDevice().getQueue("queue_transfer");
        if (!_transferQueue) {
            LUG_LOG.error("Forward::init: Can't find queue with name queue_transfer");
            return false;
        }

        API::Builder::CommandPool commandPoolBuilder(_renderer.getDevice(), *_transferQueue->getQueueFamily());
        if (!commandPoolBuilder.build(_transferCommandPool, &result)) {
            LUG_LOG.error("Forward::init: Can't create the transfer command pool: {}", result);
            return false;
        }
    }

    API::Builder::Fence fenceBuilder(_renderer.getDevice());
    fenceBuilder.setFlags(VK_FENCE_CREATE_SIGNALED_BIT); // Signaled state

    API::Builder::CommandBuffer graphicsCommandBufferBuilder(_renderer.getDevice(), _graphicsCommandPool);
    graphicsCommandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    API::Builder::CommandBuffer transferCommandBufferBuilder(_renderer.getDevice(), _transferCommandPool);
    transferCommandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    API::Builder::Semaphore semaphoreBuilder(_renderer.getDevice());

    _framesData.resize(imageViews.size());
    for (uint32_t i = 0; i < _framesData.size(); ++i) {
        // Create the render fence
        if (!fenceBuilder.build(_framesData[i].renderFence, &result)) {
            LUG_LOG.error("Forward::init: Can't create render fence: {}", result);
            return false;
        }

        // Create the transfer fence
        if (!fenceBuilder.build(_framesData[i].transferFence, &result)) {
            LUG_LOG.error("Forward::init: Can't create transfer fence: {}", result);
            return false;
        }

        // Create the render command buffer
        if (!graphicsCommandBufferBuilder.build(_framesData[i].renderCmdBuffer, &result)) {
            LUG_LOG.error("Forward::init: Can't create the render command buffer: {}", result);
            return false;
        }

        // Create the transfer command buffer
        if (!transferCommandBufferBuilder.build(_framesData[i].transferCmdBuffer, &result)) {
            LUG_LOG.error("Forward::init: Can't create the transfer command buffer: {}", result);
            return false;
        }

        if (!semaphoreBuilder.build(_framesData[i].transferSemaphore, &result)) {
            LUG_LOG.error("Forward::init: Can't create the transfer semaphore: {}", result);
            return false;
        }
    }

    if (!_cameraBufferPool) {
        _cameraBufferPool = std::make_unique<BufferPool::Camera>(_renderer);
    }

    if (!_lightBufferPool) {
        _lightBufferPool = std::make_unique<BufferPool::Light>(_renderer);
    }

    if (!_materialBufferPool) {
        _materialBufferPool = std::make_unique<BufferPool::Material>(_renderer);
    }

    if (!_cameraDescriptorSetPool) {
        _cameraDescriptorSetPool = std::make_unique<DescriptorSetPool::Camera>(_renderer);
        if (!_cameraDescriptorSetPool->init()) {
            return false;
        }
    }

    if (!_lightDescriptorSetPool) {
        _lightDescriptorSetPool = std::make_unique<DescriptorSetPool::Light>(_renderer);
        if (!_lightDescriptorSetPool->init()) {
            return false;
        }
    }

    if (!_materialDescriptorSetPool) {
        _materialDescriptorSetPool = std::make_unique<DescriptorSetPool::Material>(_renderer);
        if (!_materialDescriptorSetPool->init()) {
            return false;
        }
    }

    if (!_materialTexturesDescriptorSetPool) {
        _materialTexturesDescriptorSetPool = std::make_unique<DescriptorSetPool::MaterialTextures>(_renderer);
        if (!_materialTexturesDescriptorSetPool->init()) {
            return false;
        }
    }

    return initDepthBuffers(imageViews) && initFramebuffers(imageViews);
}

void Forward::destroy() {
    _graphicsQueue->waitIdle();
    _transferQueue->waitIdle();

    _framesData.clear();

    _depthBufferMemory.destroy();

    _cameraBufferPool.reset();
    _lightBufferPool.reset();
    _materialBufferPool.reset();

    _cameraDescriptorSetPool.reset();
    _lightDescriptorSetPool.reset();
    _materialDescriptorSetPool.reset();
    _materialTexturesDescriptorSetPool.reset();

    _graphicsCommandPool.destroy();
    _transferCommandPool.destroy();
}

bool Forward::initDepthBuffers(const std::vector<API::ImageView>& imageViews) {
    API::Builder::Image imageBuilder(_renderer.getDevice());

    imageBuilder.setUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    imageBuilder.setPreferedFormats({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT});
    imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    _framesData.resize(imageViews.size());

    API::Builder::DeviceMemory deviceMemoryBuilder(_renderer.getDevice());
    deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // Create images and add them to API::Builder::DeviceMemory
    for (uint32_t i = 0; i < imageViews.size(); ++i) {
        const VkExtent3D extent{
            /* extent.width     */ imageViews[i].getImage()->getExtent().width,
            /* extent.height    */ imageViews[i].getImage()->getExtent().height,
            /* extent.depth     */ 1
        };

        imageBuilder.setExtent(extent);

        // Create depth buffer image
        {
            VkResult result{VK_SUCCESS};
            if (!imageBuilder.build(_framesData[i].depthBuffer.image, &result)) {
                LUG_LOG.error("Forward::initDepthBuffers: Can't create depth buffer image: {}", result);
                return false;
            }

            if (!deviceMemoryBuilder.addImage(_framesData[i].depthBuffer.image)) {
                LUG_LOG.error("Forward::initDepthBuffers: Can't add image to device memory");
                return false;
            }
        }
    }

    // Initialize depth buffer memory (This memory is common for all depth buffer images)
    {
        VkResult result{VK_SUCCESS};
        if (!deviceMemoryBuilder.build(_depthBufferMemory, &result)) {
            LUG_LOG.error("Forward::initDepthBuffers: Can't create device memory: {}", result);
            return false;
        }
    }

    // Create images views
    for (uint32_t i = 0; i < imageViews.size(); ++i) {

        // Create depth buffer image view
        API::Builder::ImageView imageViewBuilder(_renderer.getDevice(), _framesData[i].depthBuffer.image);

        imageViewBuilder.setFormat(_framesData[i].depthBuffer.image.getFormat());
        imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT);

        VkResult result{VK_SUCCESS};
        if (!imageViewBuilder.build(_framesData[i].depthBuffer.imageView, &result)) {
            LUG_LOG.error("Forward::initDepthBuffers: Can't create depth buffer image view: {}", result);
            return false;
        }
    }

    return true;
}

bool Forward::initFramebuffers(const std::vector<API::ImageView>& imageViews) {
    // The lights pipelines renderpass are compatible, so we don't need to create different frame buffers for each pipeline
    const API::RenderPass* renderPass = _renderer.getPipeline(Pipeline::getBaseId())->getPipelineAPI().getRenderPass();

    _framesData.resize(imageViews.size());

    for (uint32_t i = 0; i < imageViews.size(); i++) {
        // Create depth buffer image view
        API::Builder::Framebuffer framebufferBuilder(_renderer.getDevice());

        framebufferBuilder.setRenderPass(renderPass);
        framebufferBuilder.addAttachment(&imageViews[i]);
        framebufferBuilder.addAttachment(&_framesData[i].depthBuffer.imageView);
        framebufferBuilder.setWidth(imageViews[i].getImage()->getExtent().width);
        framebufferBuilder.setHeight(imageViews[i].getImage()->getExtent().height);

        VkResult result{VK_SUCCESS};
        if (!framebufferBuilder.build(_framesData[i].framebuffer, &result)) {
            LUG_LOG.error("Forward::initFramebuffers: Can't create framebuffer: {}", result);
            return false;
        }
    }

    return true;
}

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug
