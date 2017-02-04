#pragma once

#include <unordered_map>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light.hpp>
#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/Graphics/Vulkan/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/Graphics/Vulkan/ImageView.hpp>
#include <lug/Graphics/Vulkan/RenderTechnique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API ForwardRenderTechnique final : public RenderTechnique {
public:
    struct DepthBuffer {
        std::unique_ptr<Image> image;
        std::unique_ptr<ImageView> imageView;
    };

public:
    ForwardRenderTechnique(const RenderView* renderView, const Device* device, Queue* presentQueue);

    ForwardRenderTechnique(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique(ForwardRenderTechnique&&) = delete;

    ForwardRenderTechnique& operator=(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique& operator=(ForwardRenderTechnique&&) = delete;

    ~ForwardRenderTechnique() = default;

    bool render(const RenderQueue& renderQueue, const Semaphore& imageReadySemaphore, const Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) override final;
    bool init(DescriptorPool* descriptorPool, const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;
    void destroy() override final;

    bool initLightsBuffers();
    bool initCameraBuffer(DescriptorPool* descriptorPool);
    bool initDepthBuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;
    bool initFramebuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;

private:
    std::vector<CommandBuffer> _cmdBuffers;

    // We have one depth buffer for each image
    std::vector<DepthBuffer> _depthBuffers;
    std::unique_ptr<DeviceMemory> _depthBufferMemory{nullptr};

    std::unordered_map<Light::Type, std::unique_ptr<Pipeline> > _pipelines;
    std::vector<DescriptorSet> _lightsDescriptorSets;

    std::vector<std::unique_ptr<Buffer>> _lightsBuffers;
    std::unique_ptr<DeviceMemory> _lightsBuffersMemory{nullptr};

    std::unique_ptr<DeviceMemory> _cameraBufferMemory{nullptr};
    std::unique_ptr<Buffer> _cameraBuffer{nullptr};
    DescriptorSet _cameraDescriptorSet;
};

} // Vulkan
} // Graphics
} // lug
