#pragma once

#include <lug/Graphics/Export.hpp>
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
        std::unique_ptr<DeviceMemory> imageMemory;
    };

public:
    ForwardRenderTechnique(const RenderView* renderView, const Device* device, Queue* presentQueue);

    ForwardRenderTechnique(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique(ForwardRenderTechnique&&) = delete;

    ForwardRenderTechnique& operator=(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique& operator=(ForwardRenderTechnique&&) = delete;

    ~ForwardRenderTechnique() = default;

    bool render(const RenderQueue& renderQueue, const Semaphore& imageReadySemaphore, const Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) override final;
    bool init(const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;
    void destroy() override final;

    bool initDepthBuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews);
    bool initFramebuffers(const std::vector<std::unique_ptr<ImageView> >& imageViews) override final;

private:
    std::vector<CommandBuffer> _cmdBuffers;
    std::unique_ptr<Pipeline> _graphicsPipeline{nullptr};

    // We have one depth buffer for each image
    std::vector<DepthBuffer> _depthBuffers;
};

} // Vulkan
} // Graphics
} // lug
