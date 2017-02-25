#include <chrono>
#include <algorithm>
#include <lug/Config.hpp>
#include <lug/Graphics/DirectionalLight.hpp>
#include <lug/Graphics/MeshInstance.hpp>
#include <lug/Graphics/ModelInstance.hpp>
#include <lug/Graphics/PointLight.hpp>
#include <lug/Graphics/RenderQueue.hpp>
#include <lug/Graphics/SceneNode.hpp>
#include <lug/Graphics/Spotlight.hpp>
#include <lug/Graphics/Vulkan/ForwardRenderTechnique.hpp>
#include <lug/Graphics/Vulkan/Mesh.hpp>
#include <lug/Graphics/Vulkan/Model.hpp>
#include <lug/Graphics/Vulkan/RenderView.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>
#include <lug/Math/Geometry/Transform.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

using MeshInstance = ::lug::Graphics::MeshInstance;

ForwardRenderTechnique::ForwardRenderTechnique(const RenderView* renderView, const Device* device, Queue* presentQueue) :
                                                RenderTechnique(renderView, device, presentQueue) {}

bool ForwardRenderTechnique::render(const RenderQueue& renderQueue, const Semaphore& imageReadySemaphore,
                                    const Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) {
    FrameData& frameData = _framesData[currentImageIndex];

    auto& viewport = _renderView->getViewport();

    frameData.fence.wait();
    frameData.fence.reset();
    auto& cmdBuffer = frameData.cmdBuffers[0];

    for (auto subBuffer: frameData.freeSubBuffers) {
        subBuffer->free();
    }
    frameData.freeSubBuffers.clear();

    if (!cmdBuffer.reset() || !cmdBuffer.begin()) return false;

    // Init render pass
    {
        VkViewport vkViewport{
            vkViewport.x = viewport.offset.x,
            vkViewport.y = viewport.offset.y,
            vkViewport.width = viewport.extent.width,
            vkViewport.height = viewport.extent.height,
            vkViewport.minDepth = viewport.minDepth,
            vkViewport.maxDepth = viewport.maxDepth,
        };

        VkRect2D scissor{};
        scissor.offset = {(int32_t)_renderView->getScissor().offset.x, (int32_t)_renderView->getScissor().offset.y};
        scissor.extent = {(uint32_t)_renderView->getScissor().extent.width, (uint32_t)_renderView->getScissor().extent.height};

        vkCmdSetViewport(cmdBuffer, 0, 1, &vkViewport);
        vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
    }

    // Update camera buffer data
    BufferPool::SubBuffer* cameraBuffer = _subBuffers[_renderView->getCamera()->getName()];
    {
        Camera* camera = _renderView->getCamera();
        if (camera->isDirty() && cameraBuffer) {
            frameData.freeSubBuffers.push_back(cameraBuffer);
            cameraBuffer = nullptr;
        }

        if (!cameraBuffer) {
            cameraBuffer = _cameraPool->allocate();
            _subBuffers[camera->getName()] = cameraBuffer;

            Math::Mat4x4f cameraData[] = {
                camera->getViewMatrix(),
                camera->getProjectionMatrix()
            };

            cameraBuffer->buffer->updateDataTransfer(&cmdBuffer, cameraData, sizeof(cameraData), cameraBuffer->offset);
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
                _subBuffers[light->getName()] = lightBuffer;

                uint32_t lightSize = 0;
                void* lightData;

                lightData = light->getData(lightSize);
                lightBuffer->buffer->updateDataTransfer(&cmdBuffer, lightData, lightSize, lightBuffer->offset);
            }
        }
    }

    // Render objects
    {
        // ALl the lights pipelines have the same renderPass
        RenderPass* renderPass = _pipelines[Light::Type::DirectionalLight]->getRenderPass();

        renderPass->begin(
            &cmdBuffer,
            frameData.frameBuffer,
            {viewport.extent.width, viewport.extent.height},
            {viewport.offset.x, viewport.offset.y}
        );

        cameraBuffer->descriptorSet->bind(_pipelines[Light::Type::DirectionalLight]->getLayout(), &cmdBuffer, 0, 1, &cameraBuffer->offset);

        // Blend constants are used as dst blend factor
        // We set them to 0 so that there is no blending
        {
            const float blendConstants[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            vkCmdSetBlendConstants(cmdBuffer, blendConstants);
        }


        for (std::size_t i = 0; i < renderQueue.getLightsNb(); ++i) {

            {
                if (i == 1) {
                    // Blend constants are used as dst blend factor
                    // Now the depth buffer is filled, we can set the blend constants to 1 to enable blending
                    const float blendConstants[4] = {1.0f, 1.0f, 1.0f, 1.0f};
                    vkCmdSetBlendConstants(cmdBuffer, blendConstants);
                }
            }

            auto& light = renderQueue.getLights()[i];
            auto lightType = light->getLightType();
            auto& lightPipeline = _pipelines[lightType];

            lightPipeline->bind(&cmdBuffer);

            BufferPool::SubBuffer* lightBuffer = _subBuffers[light->getName()];
            lightBuffer->descriptorSet->bind(_pipelines[Light::Type::DirectionalLight]->getLayout(), &cmdBuffer, 1, 1, &lightBuffer->offset);

            for (std::size_t j = 0; j < renderQueue.getMeshsNb(); ++j) {
                MeshInstance* meshInstance = renderQueue.getMeshs()[j];
                lug::Graphics::Mesh* mesh = static_cast<lug::Graphics::Mesh*>(meshInstance->getMesh());

                if (!mesh->isModelMesh()) {
                    Mesh* vkMesh = static_cast<Mesh*>(mesh);
                    VkBuffer vertexBuffer = *vkMesh->getVertexBuffer();
                    VkDeviceSize vertexBufferOffset = 0;
                    VkDeviceSize indexBufferOffset = 0;

                    LUG_ASSERT(meshInstance->getParent() != nullptr, "A MeshInstance should have a parent");
                    Math::Mat4x4f pushConstants[] = {
                        meshInstance->getParent()->getTransform()
                    };
                    vkCmdPushConstants(cmdBuffer, *lightPipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushConstants), pushConstants);

                    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer, &vertexBufferOffset);
                    vkCmdBindIndexBuffer(cmdBuffer, *vkMesh->getIndexBuffer(), indexBufferOffset, VK_INDEX_TYPE_UINT32);
                    vkCmdDrawIndexed(cmdBuffer, (uint32_t)vkMesh->indices.size(), 1, 0, 0, 0);
                }
                else {
                    Model::Mesh* modelMesh = static_cast<Model::Mesh*>(mesh);
                    ModelInstance* modelInstance = meshInstance->getModelInstance();
                    Model* model = static_cast<Model*>(modelInstance->getModel());
                    const VkBuffer vertexBuffer = *model->getVertexBuffer();
                    VkDeviceSize vertexBufferOffset = modelMesh->verticesOffset * sizeof(lug::Graphics::Mesh::Vertex);
                    VkDeviceSize indexBufferOffset = modelMesh->indicesOffset * sizeof(uint32_t);

                    LUG_ASSERT(modelInstance->getParent() != nullptr, "A ModelInstance should have a parent");
                    Math::Mat4x4f pushConstants[] = {
                        modelInstance->getParent()->getTransform()
                    };
                    vkCmdPushConstants(cmdBuffer, *lightPipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushConstants), pushConstants);

                    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer, &vertexBufferOffset);
                    vkCmdBindIndexBuffer(cmdBuffer, *model->getIndexBuffer(), indexBufferOffset, VK_INDEX_TYPE_UINT32);
                    vkCmdDrawIndexed(cmdBuffer, (uint32_t)modelMesh->indices.size(), 1, 0, 0, 0);
                }

            }
        }
       renderPass->end(&cmdBuffer);
    }

    if (!cmdBuffer.end()) return false;

    return _presentQueue->submit(cmdBuffer, {drawCompleteSemaphore}, {imageReadySemaphore}, {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}, frameData.fence);
}

bool ForwardRenderTechnique::init(DescriptorPool* descriptorPool, const std::vector<std::unique_ptr<ImageView> >& imageViews) {
    _pipelines[Light::Type::DirectionalLight] = Pipeline::createGraphicsPipeline(_device, "shader.vert.spv", "shader-directional.frag.spv");
    _pipelines[Light::Type::PointLight] = Pipeline::createGraphicsPipeline(_device, "shader.vert.spv", "shader-point.frag.spv");
    _pipelines[Light::Type::Spotlight] = Pipeline::createGraphicsPipeline(_device, "shader.vert.spv", "shader-spot.frag.spv");

    if (!_pipelines[Light::Type::DirectionalLight] ||
        !_pipelines[Light::Type::PointLight] ||
        !_pipelines[Light::Type::Spotlight]) {
        return false;
    }

    _framesData.resize(imageViews.size());
    for (uint32_t i = 0; i < _framesData.size(); ++i) {
        // Fence
        {
            VkFence fence;
            VkFenceCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT
            };
            VkResult result = vkCreateFence(*_device, &createInfo, nullptr, &fence);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't create swapchain fence: {}", result);
                return false;
            }

            _framesData[i].fence = Fence(fence, _device);
        }

        // Create command buffers
        {
            _framesData[i].cmdBuffers = _presentQueue->getCommandPool().createCommandBuffers();
            if (_framesData[i].cmdBuffers.size() == 0) {
                return false;
            }
        }
    }

    std::vector<uint32_t> queueFamilyIndices = {(uint32_t)_presentQueue->getFamilyIdx()};
    _cameraPool = std::make_unique<BufferPool>((uint32_t)_framesData.size(),
                                                (uint32_t)sizeof(Math::Mat4x4f) * 2,
                                                _device,
                                                queueFamilyIndices,
                                                descriptorPool,
                                                _pipelines[Light::Type::DirectionalLight]->getLayout()->getDescriptorSetLayouts()[0].get());


    uint32_t largestLightSize = 0;
    // Calculate largest light structure
    {
        largestLightSize = (uint32_t)(std::max)(sizeof(DirectionalLight::LightData), sizeof(PointLight::LightData));
        largestLightSize = (std::max)(largestLightSize, (uint32_t)sizeof(Spotlight::LightData));
    }
    _lightsPool = std::make_unique<BufferPool>((uint32_t)_framesData.size() * 50,
                                                largestLightSize,
                                                _device,
                                                queueFamilyIndices,
                                                descriptorPool,
                                                _pipelines[Light::Type::DirectionalLight]->getLayout()->getDescriptorSetLayouts()[1].get());

    return initDepthBuffers(imageViews) && initFramebuffers(imageViews);
}

void ForwardRenderTechnique::destroy() {
    _presentQueue->waitIdle();

    for (auto& pipeline: _pipelines) {
        pipeline.second->destroy();
    }

    _framesData.clear();
}

bool ForwardRenderTechnique::initDepthBuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) {
    VkFormat imagesFormat = Image::findSupportedFormat(_device,
                                                       {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                                       VK_IMAGE_TILING_OPTIMAL,
                                                       VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    if (imagesFormat == VK_FORMAT_UNDEFINED) {
        LUG_LOG.error("ForwardRenderTechnique: Can't find supported format for depth buffer");
        return false;
    }

    if (_depthBufferMemory) {
        _depthBufferMemory.reset();
    }

    _framesData.resize(imageViews.size());
    for (uint32_t i = 0; i < imageViews.size(); ++i) {
        std::unique_ptr<Image> image = nullptr;
        std::unique_ptr<ImageView> imageView = nullptr;

        VkExtent3D extent {
            extent.width = imageViews[i]->getExtent().width,
            extent.height = imageViews[i]->getExtent().height,
            extent.depth = 1
        };

        // Create depth buffer image
        {
            image = Image::create(_device, imagesFormat, extent, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
            if (!image) {
                LUG_LOG.error("ForwardRenderTechnique: Can't create depth buffer image");
                return false;
            }

            auto& imageRequirements = image->getRequirements();

            // Initialize depth buffer memory (This memory is common for all depth buffer images)
            if (!_depthBufferMemory) {
                uint32_t memoryTypeIndex = DeviceMemory::findMemoryType(_device, imageRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
                // Allocate image requirements size for all images
                _depthBufferMemory = DeviceMemory::allocate(_device, imageRequirements.size * imageViews.size(), memoryTypeIndex);
                if (!_depthBufferMemory) {
                    LUG_LOG.error("ForwardRenderTechnique: Can't allocate device memory for depth buffer images");
                    return false;
                }
            }

            // Bind memory to image
            image->bindMemory(_depthBufferMemory.get(), imageRequirements.size * i);
        }

        // Create depth buffer image view
        {
            imageView = ImageView::create(_device, image.get(), imagesFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
            if (!imageView) {
                LUG_LOG.error("ForwardRenderTechnique: Can't create depth buffer image view");
                return false;
            }
        }

        _framesData[i].depthBuffer.image = std::move(image);
        _framesData[i].depthBuffer.imageView = std::move(imageView);
    }
    return true;
}

bool ForwardRenderTechnique::initFramebuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) {
    // The lights pipelines renderpass are compatible, so we don't need to create different frame buffers for each pipeline
    RenderPass* renderPass = _pipelines[Light::Type::DirectionalLight]->getRenderPass();

    VkResult result;
    _framesData.resize(imageViews.size());

    for (size_t i = 0; i < imageViews.size(); i++) {
        VkImageView attachments[2]{
            *imageViews[i],
            *_framesData[i].depthBuffer.imageView
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = *renderPass;
        framebufferInfo.attachmentCount = 2;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = imageViews[i]->getExtent().width;
        framebufferInfo.height = imageViews[i]->getExtent().height;
        framebufferInfo.layers = 1;

        VkFramebuffer fb;
        result = vkCreateFramebuffer(*_device, &framebufferInfo, nullptr, &fb);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Failed to create framebuffer: {}", result);
            return false;
        }
        // TODO: Remove the extent initializer list when struct Extent is externalised
        _framesData[i].frameBuffer.destroy();
        _framesData[i].frameBuffer = Framebuffer(fb, _device, {imageViews[i]->getExtent().width, imageViews[i]->getExtent().height});
    }
    return true;
}

} // Vulkan
} // Graphics
} // lug
