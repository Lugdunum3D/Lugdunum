#include <lug/Graphics/Vulkan/Render/Technique/Forward.hpp>

#include <algorithm>

#include <lug/Config.hpp>
#include <lug/Graphics/Light/Directional.hpp>
#include <lug/Graphics/Light/Point.hpp>
#include <lug/Graphics/Light/Spot.hpp>
#include <lug/Graphics/Render/Queue.hpp>
#include <lug/Graphics/Scene/MeshInstance.hpp>
#include <lug/Graphics/Scene/ModelInstance.hpp>
#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Image.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Vulkan/Render/Camera.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/Render/Model.hpp>
#include <lug/Graphics/Vulkan/Render/View.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>
#include <lug/Math/Geometry/Transform.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace Technique {

using MeshInstance = ::lug::Graphics::Scene::MeshInstance;

Forward::Forward(const Renderer& renderer, const Render::View& renderView) :
    Technique(renderer, renderView) {}

bool Forward::render(
    const ::lug::Graphics::Render::Queue& renderQueue,
    const API::Semaphore& imageReadySemaphore,
    const API::Semaphore& drawCompleteSemaphore,
    uint32_t currentImageIndex) {
    FrameData& frameData = _framesData[currentImageIndex];

    auto& viewport = _renderView.getViewport();

    frameData.fence.wait();
    frameData.fence.reset();
    auto& cmdBuffer = frameData.cmdBuffers[0];

    for (auto subBuffer: frameData.freeSubBuffers) {
        subBuffer->free();
    }

    frameData.freeSubBuffers.clear();

    if (!cmdBuffer.reset() || !cmdBuffer.begin()) {
        return false;
    }

    // Init render pass
    {
        const VkViewport vkViewport{
            /* vkViewport.x */ viewport.offset.x,
            /* vkViewport.y */ viewport.offset.y,
            /* vkViewport.width */ viewport.extent.width,
            /* vkViewport.height */ viewport.extent.height,
            /* vkViewport.minDepth */ viewport.minDepth,
            /* vkViewport.maxDepth */ viewport.maxDepth,
        };

        const VkRect2D scissor{
            /* scissor.offset */ {
                (int32_t)_renderView.getScissor().offset.x,
                (int32_t)_renderView.getScissor().offset.y},
            /* scissor.extent */ {
                (uint32_t)_renderView.getScissor().extent.width,
                (uint32_t)_renderView.getScissor().extent.height
            }
        };

        cmdBuffer.setViewport({vkViewport});
        cmdBuffer.setScissor({scissor});
    }

    // Update camera buffer data
    BufferPool::SubBuffer* cameraBuffer = _subBuffers[_renderView.getCamera()->getName()];
    {
        Camera* camera = static_cast<Camera*>(_renderView.getCamera());

        if (camera->isDirty() && cameraBuffer) {
            frameData.freeSubBuffers.push_back(cameraBuffer);
            cameraBuffer = nullptr;
        }

        if (!cameraBuffer) {
            cameraBuffer = _cameraPool->allocate();
            if (!cameraBuffer) {
                return false;
            }

            _subBuffers[camera->getName()] = cameraBuffer;

            const Math::Mat4x4f cameraData[] = {
                camera->getViewMatrix(),
                camera->getProjectionMatrix()
            };

            cmdBuffer.updateBuffer(*cameraBuffer->buffer, cameraData, sizeof(cameraData), cameraBuffer->offset);
            camera->isDirty(false);
        }
    }

    // Update lights buffers data
    {
        for (std::size_t i = 0; i < renderQueue.getLightsNb(); ++i) {
            auto& light = renderQueue.getLights()[i];

            BufferPool::SubBuffer* lightBuffer = _subBuffers[light->getName()];

            if (light->isDirty() && lightBuffer) {
                frameData.freeSubBuffers.push_back(lightBuffer);
                lightBuffer = nullptr;
            }

            if (!lightBuffer) {
                lightBuffer = _lightsPool->allocate();
                if (!lightBuffer) {
                    return false;
                }

                _subBuffers[light->getName()] = lightBuffer;

                uint32_t lightSize = 0;
                void* lightData;

                lightData = light->getData(lightSize);
                cmdBuffer.updateBuffer(*lightBuffer->buffer, lightData, lightSize, lightBuffer->offset);
            }
        }
    }

    // Render objects
    {
        // All the lights pipelines have the same renderPass
        const API::RenderPass* renderPass = _pipelines[Light::Light::Type::Directional].getRenderPass();

        API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
            /* beginRenderPass.framebuffer */ frameData.framebuffer,
            /* beginRenderPass.renderArea */ {},
            /* beginRenderPass.clearValues */ {}
        };

        beginRenderPass.renderArea.offset = {static_cast<int32_t>(viewport.offset.x), static_cast<int32_t>(viewport.offset.y)};
        beginRenderPass.renderArea.extent = {static_cast<uint32_t>(viewport.extent.width), static_cast<uint32_t>(viewport.extent.height)};

        beginRenderPass.clearValues.resize(2);
        beginRenderPass.clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        beginRenderPass.clearValues[1].depthStencil = {1.0f, 0};

        cmdBuffer.beginRenderPass(*renderPass, beginRenderPass);

        const API::CommandBuffer::CmdBindDescriptors cameraBind {
            /* cameraBind.pipelineLayout */ *_pipelines[Light::Light::Type::Directional].getLayout(),
            /* cameraBind.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* cameraBind.firstSet */ 0,
            /* cameraBind.descriptorSets */ {cameraBuffer->descriptorSet},
            /* cameraBind.dynamicOffsets */ {cameraBuffer->offset},
        };

        cmdBuffer.bindDescriptorSets(cameraBind);

        // Blend constants are used as dst blend factor
        // We set them to 0 so that there is no blending
        {
            const float blendConstants[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            cmdBuffer.setBlendConstants(blendConstants);
        }

        for (std::size_t i = 0; i < renderQueue.getLightsNb(); ++i) {

            {
                if (i == 1) {
                    // Blend constants are used as dst blend factor
                    // Now the depth buffer is filled, we can set the blend constants to 1 to enable blending
                    const float blendConstants[4] = {1.0f, 1.0f, 1.0f, 1.0f};
                    cmdBuffer.setBlendConstants(blendConstants);
                }
            }

            auto& light = renderQueue.getLights()[i];
            auto lightType = light->getLightType();
            auto& lightPipeline = _pipelines[lightType];

            cmdBuffer.bindPipeline(lightPipeline);

            BufferPool::SubBuffer* lightBuffer = _subBuffers[light->getName()];

            const API::CommandBuffer::CmdBindDescriptors lightBind {
                /* cameraBind.pipelineLayout */ *_pipelines[Light::Light::Type::Directional].getLayout(),
                /* lightBind.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                /* lightBind.firstSet */ 1,
                /* lightBind.descriptorSets */ {lightBuffer->descriptorSet},
                /* lightBind.dynamicOffsets */ {lightBuffer->offset},
            };

            cmdBuffer.bindDescriptorSets(lightBind);

            for (std::size_t j = 0; j < renderQueue.getMeshsNb(); ++j) {
                MeshInstance* meshInstance = renderQueue.getMeshs()[j];
                lug::Graphics::Render::Mesh* mesh = static_cast<lug::Graphics::Render::Mesh*>(meshInstance->getMesh());

                if (!mesh->isModelMesh()) {
                    Mesh* vkMesh = static_cast<Mesh*>(mesh);
                    VkDeviceSize vertexBufferOffset = 0;
                    VkDeviceSize indexBufferOffset = 0;

                    LUG_ASSERT(meshInstance->getParent() != nullptr, "A MeshInstance should have a parent");

                    const Math::Mat4x4f pushConstants[] = {
                        meshInstance->getParent()->getTransform()
                    };

                    const API::CommandBuffer::CmdPushConstants cmdPushConstants{
                        /* cmdPushConstants.layout */ static_cast<VkPipelineLayout>(*lightPipeline.getLayout()),
                        /* cmdPushConstants.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT,
                        /* cmdPushConstants.offset */ 0,
                        /* cmdPushConstants.size */ sizeof(pushConstants),
                        /* cmdPushConstants.values */ pushConstants
                    };
                    cmdBuffer.pushConstants(cmdPushConstants);

                    cmdBuffer.bindVertexBuffers({vkMesh->getVertexBuffer()}, {vertexBufferOffset});
                    cmdBuffer.bindIndexBuffer(*vkMesh->getIndexBuffer(), VK_INDEX_TYPE_UINT32, indexBufferOffset);

                    const API::CommandBuffer::CmdDrawIndexed cmdDrawIndexed {
                        /* cmdDrawIndexed.indexCount */ static_cast<uint32_t>(vkMesh->indices.size()),
                        /* cmdDrawIndexed.instanceCount */ 1,
                    };

                    cmdBuffer.drawIndexed(cmdDrawIndexed);
                } else {
                    Model::Mesh* modelMesh = static_cast<Model::Mesh*>(mesh);
                    Scene::ModelInstance* modelInstance = meshInstance->getModelInstance();
                    Model* model = static_cast<Model*>(modelInstance->getModel());
                    VkDeviceSize vertexBufferOffset = modelMesh->verticesOffset * sizeof(lug::Graphics::Render::Mesh::Vertex);
                    VkDeviceSize indexBufferOffset = modelMesh->indicesOffset * sizeof(uint32_t);

                    LUG_ASSERT(modelInstance->getParent() != nullptr, "A ModelInstance should have a parent");

                    const Math::Mat4x4f pushConstants[] = {
                        modelInstance->getParent()->getTransform()
                    };

                    const API::CommandBuffer::CmdPushConstants cmdPushConstants{
                        /* cmdPushConstants.layout */ static_cast<VkPipelineLayout>(*lightPipeline.getLayout()),
                        /* cmdPushConstants.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT,
                        /* cmdPushConstants.offset */ 0,
                        /* cmdPushConstants.size */ sizeof(pushConstants),
                        /* cmdPushConstants.values */ pushConstants
                    };
                    cmdBuffer.pushConstants(cmdPushConstants);

                    cmdBuffer.bindVertexBuffers({model->getVertexBuffer()}, {vertexBufferOffset});
                    cmdBuffer.bindIndexBuffer(*model->getIndexBuffer(), VK_INDEX_TYPE_UINT32, indexBufferOffset);

                    const API::CommandBuffer::CmdDrawIndexed cmdDrawIndexed {
                        /* cmdDrawIndexed.indexCount */ static_cast<uint32_t>(modelMesh->indices.size()),
                        /* cmdDrawIndexed.instanceCount */ 1,
                    };

                    cmdBuffer.drawIndexed(cmdDrawIndexed);
                }
            }
        }

        cmdBuffer.endRenderPass();
    }

    if (!cmdBuffer.end()) {
        return false;
    }

    return _graphicsQueue->submit(
        cmdBuffer,
        {static_cast<VkSemaphore>(drawCompleteSemaphore)},
        {static_cast<VkSemaphore>(imageReadySemaphore)},
        {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
        static_cast<VkFence>(frameData.fence)
    );
}

bool Forward::init(API::DescriptorPool* descriptorPool, const std::vector<API::ImageView>& imageViews) {
    auto colorFormat = _renderView.getFormat().format;

    {
        auto initLightPipeline = [this, &colorFormat](Light::Light::Type lightType, const char* vertexShader, const char* fragmentShader) {
            API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

            // Set shaders state
            if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", _renderer.getInfo().shadersRoot + vertexShader)
                || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", _renderer.getInfo().shadersRoot + fragmentShader)) {
                LUG_LOG.error("ForwardRenderer: Can't create pipeline's shaders.");
                return false;
            }

            // Set vertex input state
            auto vertexBinding = graphicsPipelineBuilder.addInputBinding(sizeof(Render::Mesh::Vertex), VK_VERTEX_INPUT_RATE_VERTEX);

            vertexBinding.addAttributes(VK_FORMAT_R32G32B32_SFLOAT, 0); // Position
            vertexBinding.addAttributes(VK_FORMAT_R32G32B32_SFLOAT, offsetof(Render::Mesh::Vertex, color)); // Color
            vertexBinding.addAttributes(VK_FORMAT_R32G32B32_SFLOAT, offsetof(Render::Mesh::Vertex, normal)); // Normal
            vertexBinding.addAttributes(VK_FORMAT_R32G32_SFLOAT, offsetof(Render::Mesh::Vertex, uv)); // UV

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
                /* scissor.offset */ {0, 0},
                /* scissor.extent */ {0, 0}
            };

            auto viewportState = graphicsPipelineBuilder.getViewportState();
            viewportState.addViewport(viewport);
            viewportState.addScissor(scissor);

            // Set rasterization state
            auto rasterizationState = graphicsPipelineBuilder.getRasterizationState();
            rasterizationState.setFrontFace(VK_FRONT_FACE_CLOCKWISE);

            // Set depth stencil state
            auto depthStencilState = graphicsPipelineBuilder.getDepthStencilState();
            depthStencilState.enableDepthTest(VK_COMPARE_OP_LESS_OR_EQUAL);
            depthStencilState.enableDepthWrite();

            // Set color blend state
            const VkPipelineColorBlendAttachmentState colorBlendAttachment{
                /* colorBlendAttachment.blendEnable */ VK_TRUE,
                /* colorBlendAttachment.srcColorBlendFactor */ VK_BLEND_FACTOR_ONE,
                /* colorBlendAttachment.dstColorBlendFactor */ VK_BLEND_FACTOR_CONSTANT_COLOR,
                /* colorBlendAttachment.colorBlendOp */ VK_BLEND_OP_ADD,
                /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ZERO,
                /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_CONSTANT_COLOR,
                /* colorBlendAttachment.alphaBlendOp */ VK_BLEND_OP_ADD,
                /* colorBlendAttachment.colorWriteMask */ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
            };

            auto colorBlendState = graphicsPipelineBuilder.getColorBlendState();
            colorBlendState.addAttachment(colorBlendAttachment);

            // Set dynamic states
            graphicsPipelineBuilder.setDynamicStates({
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
                VK_DYNAMIC_STATE_BLEND_CONSTANTS
            });

            // Set pipeline layout
            {
                VkResult result{VK_SUCCESS};
                std::vector<API::DescriptorSetLayout> descriptorSetLayouts(2);
                API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(_renderer.getDevice());

                // Bindings set 0
                {
                    // Camera uniform buffer
                    const VkDescriptorSetLayoutBinding binding{
                        /* binding.binding */ 0,
                        /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                        /* binding.descriptorCount */ 1,
                        /* binding.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                        /* binding.pImmutableSamplers */ nullptr
                    };

                    descriptorSetLayoutBuilder.setBindings({binding});
                    if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                        LUG_LOG.error("ForwardRenderer: Can't create pipeline descriptor sets layout 0: {}", result);
                        return false;
                    }
                }

                // Bindings set 1
                {
                    // Light uniform buffer
                    const VkDescriptorSetLayoutBinding binding{
                        /* binding.binding */ 0,
                        /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                        /* binding.descriptorCount */ 1,
                        /* binding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                        /* binding.pImmutableSamplers */ nullptr
                    };

                    descriptorSetLayoutBuilder.setBindings({binding});
                    if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[1], &result)) {
                        LUG_LOG.error("ForwardRenderer: Can't create pipeline descriptor sets layout 1: {}", result);
                        return false;
                    }
                }

                // Model transformation
                const VkPushConstantRange pushConstant{
                    /* pushConstant.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT,
                    /* pushConstant.offset */ 0,
                    /* pushConstant.size */ sizeof(Math::Mat4x4f)
                };

                API::Builder::PipelineLayout pipelineLayoutBuilder(_renderer.getDevice());

                pipelineLayoutBuilder.setPushConstants({pushConstant});
                pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

                API::PipelineLayout pipelineLayout;
                if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
                    LUG_LOG.error("ForwardRenderer: Can't create pipeline layout: {}", result);
                    return false;
                }

                graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
            }

            // Set render pass
            {
                API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

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

                const VkFormat depthFormat = API::Image::findSupportedFormat(
                    _renderer.getDevice(),
                    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
                );

                const VkAttachmentDescription depthAttachment{
                    /* depthAttachment.flags */ 0,
                    /* depthAttachment.format */ depthFormat,
                    /* depthAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
                    /* depthAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_CLEAR,
                    /* depthAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
                    /* depthAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                    /* depthAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
                    /* depthAttachment.initialLayout */ VK_IMAGE_LAYOUT_UNDEFINED,
                    /* depthAttachment.finalLayout */ VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                };

                auto depthAttachmentIndex = renderPassBuilder.addAttachment(depthAttachment);

                const API::Builder::RenderPass::SubpassDescription subpassDescription{
                    /* subpassDescription.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
                    /* subpassDescription.inputAttachments */ {},
                    /* subpassDescription.colorAttachments */ {{colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
                    /* subpassDescription.resolveAttachments */ {},
                    /* subpassDescription.depthStencilAttachment */ {depthAttachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
                    /* subpassDescription.preserveAttachments */ {},
                };

                renderPassBuilder.addSubpass(subpassDescription);

                VkResult result{VK_SUCCESS};
                API::RenderPass renderPass;
                if (!renderPassBuilder.build(renderPass, &result)) {
                    LUG_LOG.error("ForwardRenderer: Can't create render pass: {}", result);
                    return false;
                }

                graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
            }

            VkResult result{VK_SUCCESS};
            if (!graphicsPipelineBuilder.build(_pipelines[lightType], &result)) {
                LUG_LOG.error("ForwardRenderer: Can't create pipeline: {}", result);
                return false;
            }

            return true;
        };

        if (!initLightPipeline(Light::Light::Type::Directional, "shader.vert.spv", "shader-directional.frag.spv")
            || !initLightPipeline(Light::Light::Type::Point, "shader.vert.spv", "shader-point.frag.spv")
            || !initLightPipeline(Light::Light::Type::Spot, "shader.vert.spv", "shader-spot.frag.spv")) {
            return false;
        }
    }

    _framesData.resize(imageViews.size());

    const API::QueueFamily* graphicsQueueFamily = _renderer.getDevice().getQueueFamily(VK_QUEUE_GRAPHICS_BIT);
    if (!graphicsQueueFamily) {
        LUG_LOG.error("Forward::init: Can't find VK_QUEUE_GRAPHICS_BIT queue family");
        return false;
    }
    _graphicsQueue = graphicsQueueFamily->getQueue("queue_graphics");
    if (!_graphicsQueue) {
        LUG_LOG.error("Forward::init: Can't find queue with name queue_graphics");
        return false;
    }

    API::Builder::CommandPool commandPoolBuilder(_renderer.getDevice(), *graphicsQueueFamily);
    VkResult result{VK_SUCCESS};
    if (!commandPoolBuilder.build(_commandPool, &result)) {
        LUG_LOG.error("Forward::init: Can't create a command pool: {}", result);
        return false;
    }

    API::Builder::Fence fenceBuilder(_renderer.getDevice());
    fenceBuilder.setFlags(VK_FENCE_CREATE_SIGNALED_BIT); // Signaled state

    API::Builder::CommandBuffer commandBufferBuilder(_renderer.getDevice(), _commandPool);
    commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    for (uint32_t i = 0; i < _framesData.size(); ++i) {
        // Create the Fence
        if (!fenceBuilder.build(_framesData[i].fence, &result)) {
            LUG_LOG.error("Forward::init: Can't create swapchain fence: {}", result);
            return false;
        }

        // Create command buffers
        _framesData[i].cmdBuffers.resize(1); // The builder will build according to the array size.

        if (!commandBufferBuilder.build(_framesData[i].cmdBuffers, &result)) {
            LUG_LOG.error("Forward::init: Can't create the command buffer: {}", result);
            return false;
        }
    }

    std::set<uint32_t> queueFamilyIndices = {graphicsQueueFamily->getIdx()};
    _cameraPool = std::make_unique<BufferPool>(
        (uint32_t)_framesData.size(),
        (uint32_t)sizeof(Math::Mat4x4f) * 2,
        _renderer.getDevice(),
        queueFamilyIndices,
        *descriptorPool,
        &_pipelines[Light::Light::Type::Directional].getLayout()->getDescriptorSetLayouts()[0]
    );

    uint32_t largestLightSize = 0;
    // Calculate largest light structure
    {
        largestLightSize = (uint32_t)(std::max)(sizeof(Light::Directional::LightData), sizeof(Light::Point::LightData));
        largestLightSize = (std::max)(largestLightSize, (uint32_t)sizeof(Light::Spot::LightData));
    }

    _lightsPool = std::make_unique<BufferPool>(
        (uint32_t)_framesData.size() * 50,
        largestLightSize,
        _renderer.getDevice(),
        queueFamilyIndices,
        *descriptorPool,
        &_pipelines[Light::Light::Type::Directional].getLayout()->getDescriptorSetLayouts()[1]
    );

    return initDepthBuffers(imageViews) && initFramebuffers(imageViews);
}

void Forward::destroy() {
    _graphicsQueue->waitIdle();

    for (auto& pipeline: _pipelines) {
        pipeline.second.destroy();
    }

    _framesData.clear();

    _depthBufferMemory.destroy();

    _cameraPool.reset();
    _lightsPool.reset();
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
            /* extent.width */ imageViews[i].getImage()->getExtent().width,
            /* extent.height */ imageViews[i].getImage()->getExtent().height,
            /* extent.depth */ 1
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
    const API::RenderPass* renderPass = _pipelines[Light::Light::Type::Directional].getRenderPass();

    _framesData.resize(imageViews.size());

    for (size_t i = 0; i < imageViews.size(); i++) {
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
