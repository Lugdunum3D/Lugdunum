#pragma once

#include <unordered_map>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light.hpp>
#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/Graphics/Vulkan/BufferPool.hpp>
#include <lug/Graphics/Vulkan/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Fence.hpp>
#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/Graphics/Vulkan/ImageView.hpp>
#include <lug/Graphics/Vulkan/RenderTechnique.hpp>
#include <lug/System/Clock.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API ForwardRenderTechnique final : public RenderTechnique {
private:
    struct DepthBuffer {
        std::unique_ptr<Image> image;
        std::unique_ptr<ImageView> imageView;
    };

    struct FrameData {
        DepthBuffer depthBuffer;
        Framebuffer frameBuffer;
        Fence fence;

        // There is actually only 1 command buffer
        std::vector<CommandBuffer> cmdBuffers;

        std::vector<BufferPool::SubBuffer*> freeSubBuffers;
    };

public:
    ForwardRenderTechnique(const Renderer& renderer, const RenderView* renderView, const Device* device, Queue* presentQueue);

    ForwardRenderTechnique(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique(ForwardRenderTechnique&&) = delete;

    ForwardRenderTechnique& operator=(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique& operator=(ForwardRenderTechnique&&) = delete;

    ~ForwardRenderTechnique() = default;

    bool render(const RenderQueue& renderQueue, const Semaphore& imageReadySemaphore, const Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) override final;
    bool init(DescriptorPool* descriptorPool, const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;
    void destroy() override final;

    bool initDepthBuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;
    bool initFramebuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;

private:
    std::unique_ptr<BufferPool> _cameraPool;
    std::unique_ptr<BufferPool> _lightsPool;

    std::unique_ptr<DeviceMemory> _depthBufferMemory{nullptr};

    std::unordered_map<Light::Type, std::unique_ptr<Pipeline> > _pipelines;

    std::vector<FrameData> _framesData;

    std::unordered_map<std::string, BufferPool::SubBuffer*> _subBuffers;
};

} // Vulkan
} // Graphics
} // lug
