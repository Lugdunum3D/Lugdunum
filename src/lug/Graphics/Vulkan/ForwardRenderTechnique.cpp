#include <lug/Config.hpp>
#include <lug/Graphics/MeshInstance.hpp>
#include <lug/Graphics/RenderQueue.hpp>
#include <lug/Graphics/SceneNode.hpp>
#include <lug/Graphics/Vulkan/ForwardRenderTechnique.hpp>
#include <lug/Graphics/Vulkan/Mesh.hpp>
#include <lug/Graphics/Vulkan/RenderView.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Geometry/Transform.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

using MeshInstance = ::lug::Graphics::MeshInstance;

ForwardRenderTechnique::ForwardRenderTechnique(const RenderView* renderView, const Device* device, Queue* presentQueue) :
                                                RenderTechnique(renderView, device, presentQueue) {}

bool ForwardRenderTechnique::render(const RenderQueue& renderQueue, const Semaphore& imageReadySemaphore,
                                    const Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) {
    static Math::Mat4x4f projectionMatrix{Math::Mat4x4f::identity()};
    static Math::Mat4x4f viewMatrix{Math::Geometry::lookAt<float>({0.0f, -4.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f})};
    static Math::Mat4x4f vpMatrix{Math::Mat4x4f::identity()};

    auto& viewport = _renderView->getViewport();

    // Update the projection matrix and rotate model matrix
    {
        projectionMatrix = Math::Geometry::perspective(
            Math::Geometry::radians(_renderView->getCamera()->getFov()),
            viewport.getRatio(),
            0.1f, 100.0f);

        // Compute the VP Matrix
        vpMatrix = projectionMatrix * viewMatrix;
    }

    if (!_cmdBuffers[0].begin()) return false;

    // Init render pass
    {
        _graphicsPipeline->bind(&_cmdBuffers[0]);

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

        vkCmdSetViewport(_cmdBuffers[0], 0, 1, &vkViewport);
        vkCmdSetScissor(_cmdBuffers[0], 0, 1, &scissor);
    }

    // Render objects
    {
        VkDeviceSize vertexBufferOffset = 0;
        VkDeviceSize indexBufferOffset = 0;
        _graphicsPipeline->getRenderPass()->begin(&_cmdBuffers[0],
                                                _framebuffers[currentImageIndex],
                                                {viewport.extent.width, viewport.extent.height},
                                                {viewport.offset.x, viewport.offset.y});

        for (auto& object: renderQueue.getObjects()) {
            const MeshInstance* meshInstance = dynamic_cast<const MeshInstance*>(object);
            if (meshInstance) {
                const Mesh* mesh = static_cast<const Mesh*>(meshInstance->getMesh());
                const VkBuffer vertexBuffer = *mesh->getVertexBuffer();

                LUG_ASSERT(object->getParent() != nullptr, "A MovableObject should have a parent");
                Math::Mat4x4f pushConstants[] = {
                    vpMatrix,
                    object->getParent()->getTransform()
                };
                vkCmdPushConstants(_cmdBuffers[0], *_graphicsPipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushConstants), pushConstants);


                vkCmdBindVertexBuffers(_cmdBuffers[0], 0, 1, &vertexBuffer, &vertexBufferOffset);
                vkCmdBindIndexBuffer(_cmdBuffers[0], *mesh->getIndexBuffer(), indexBufferOffset, VK_INDEX_TYPE_UINT32);
                vkCmdDrawIndexed(_cmdBuffers[0], (uint32_t)mesh->indices.size(), 1, 0, 0, 0);
            }
        }

         _graphicsPipeline->getRenderPass()->end(&_cmdBuffers[0]);
    }

    if (!_cmdBuffers[0].end()) return false;

    return _presentQueue->submit(_cmdBuffers[0], {drawCompleteSemaphore}, {imageReadySemaphore}, {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT});
}

bool ForwardRenderTechnique::init(const std::vector<std::unique_ptr<ImageView> >& imageViews) {
    _graphicsPipeline = Pipeline::createGraphicsPipeline(_device);
    if (!_graphicsPipeline) {
        return false;
    }

    _cmdBuffers = _presentQueue->getCommandPool().createCommandBuffers();
    if (_cmdBuffers.size() == 0) {
        return false;
    }

    return initDepthBuffers(imageViews) && initFramebuffers(imageViews);
}

void ForwardRenderTechnique::destroy() {
    _presentQueue->waitIdle();

    if (_graphicsPipeline) {
        _graphicsPipeline->destroy();
    }

    for (auto& cmdBuffer: _cmdBuffers) {
        cmdBuffer.destroy();
    }

    _framebuffers.clear();
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

    _depthBuffers.resize(imageViews.size());
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

        _depthBuffers[i].image = std::move(image);
        _depthBuffers[i].imageView = std::move(imageView);
    }
    return true;
}

bool ForwardRenderTechnique::initFramebuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) {
    RenderPass* renderPass = _graphicsPipeline->getRenderPass();

    VkResult result;
    _framebuffers.clear();
    _framebuffers.resize(imageViews.size());

    for (size_t i = 0; i < imageViews.size(); i++) {
        VkImageView attachments[2]{
            *imageViews[i],
            *_depthBuffers[i].imageView
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
        _framebuffers[i] = Framebuffer(fb, _device, {imageViews[i]->getExtent().width, imageViews[i]->getExtent().height});
    }
    return true;
}

} // Vulkan
} // Graphics
} // lug
