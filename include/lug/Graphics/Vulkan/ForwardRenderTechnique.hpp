#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/RenderTechnique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API ForwardRenderTechnique final : public RenderTechnique {
public:
    ForwardRenderTechnique(const RenderView* renderView, const Device* device, Queue* presentQueue);

    ForwardRenderTechnique(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique(ForwardRenderTechnique&&) = delete;

    ForwardRenderTechnique& operator=(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique& operator=(ForwardRenderTechnique&&) = delete;

    ~ForwardRenderTechnique() = default;

    bool render(const RenderQueue& renderQueue, const Semaphore& imageReadySemaphore, const Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) override final;
    bool init(const std::vector<ImageView>& imageViews) override final;
    void destroy() override final;

    bool initFramebuffers(const std::vector<ImageView>& imageViews) override final;

private:
    std::vector<CommandBuffer> _cmdBuffers;
    std::unique_ptr<Pipeline> _graphicsPipeline{nullptr};
};

} // Vulkan
} // Graphics
} // lug
