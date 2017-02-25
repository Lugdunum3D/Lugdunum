#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/ImageView.hpp>
#include <lug/Graphics/Vulkan/Pipeline.hpp>
#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/Graphics/Vulkan/Semaphore.hpp>

namespace lug {
namespace Graphics {

class RenderQueue;

namespace Vulkan {

class Renderer;
class RenderView;

class LUG_GRAPHICS_API RenderTechnique {
public:
    RenderTechnique(const Renderer& renderer, const RenderView* renderView, const Device* device, Queue* presentQueue);

    RenderTechnique(const RenderTechnique&) = delete;
    RenderTechnique(RenderTechnique&&) = delete;

    RenderTechnique& operator=(const RenderTechnique&) = delete;
    RenderTechnique& operator=(RenderTechnique&&) = delete;

    virtual ~RenderTechnique() = default;

    virtual bool render(const RenderQueue& renderQueue, const Semaphore& imageReadySemaphore, const Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) = 0;
    virtual bool init(DescriptorPool* descriptorPool, const std::vector<std::unique_ptr<ImageView> >& imageViews) = 0;
    virtual void destroy() = 0;

    virtual bool initDepthBuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) = 0;
    virtual bool initFramebuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) = 0;

protected:
    const Renderer& _renderer;
    const RenderView* _renderView;
    const Device* _device{nullptr};

    Queue* _presentQueue{nullptr};
};

} // Vulkan
} // Graphics
} // lug
