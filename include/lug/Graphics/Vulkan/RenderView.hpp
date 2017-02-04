#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderView.hpp>
#include <lug/Graphics/Vulkan/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/Graphics/Vulkan/RenderTechnique.hpp>
#include <lug/Graphics/Vulkan/Semaphore.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API RenderView final : public lug::Graphics::RenderView {
public:
    // TODO: Take the number of images and the list of images
    RenderView(const RenderTarget* renderTarget);

    RenderView(const RenderView&) = delete;
    RenderView(RenderView&&) = delete;

    RenderView& operator=(const RenderView&) = delete;
    RenderView& operator=(RenderView&&) = delete;

    ~RenderView() = default;

    bool init(RenderView::InitInfo& initInfo,
                const Device* device,
                Queue* presentQueue,
                DescriptorPool* descriptorPool,
                const std::vector<std::unique_ptr<ImageView> >& imageViews);

    bool render(const Semaphore& imageReadySemaphore, uint32_t currentImageIndex);
    void destroy() override final;

    RenderTechnique* getRenderTechnique();
    const Semaphore& getDrawCompleteSemaphore() const;

    // TODO: Add a method to change the index of the good image to use (change by the render window)
    // TODO: Add the semaphores for the images ready in that class too

private:
    std::unique_ptr<RenderTechnique> _renderTechnique{nullptr};

    Semaphore _drawCompleteSemaphore{};
    Queue* _presentQueue;
};

#include <lug/Graphics/Vulkan/RenderView.inl>

} // Vulkan
} // Graphics
} // lug
