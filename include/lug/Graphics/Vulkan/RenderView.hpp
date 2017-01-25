#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderView.hpp>
#include <lug/Graphics/Vulkan/RenderTechnique.hpp>

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

    void init(RenderView::InitInfo& initInfo);

    void render() override final;

    // TODO: Add a method to change the index of the good image to use (change by the render window)
    // TODO: Add the semaphores for the images ready in that class too

private:
    std::unique_ptr<RenderTechnique> _renderTechnique{nullptr};
};

} // Vulkan
} // Graphics
} // lug
