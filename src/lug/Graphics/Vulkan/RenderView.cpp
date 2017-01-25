#include <lug/Graphics/Vulkan/RenderView.hpp>
#include <lug/Graphics/RenderQueue.hpp>
#include <lug/Graphics/Vulkan/ForwardRenderTechnique.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

RenderView::RenderView(const RenderTarget* renderTarget) : lug::Graphics::RenderView(renderTarget) {}

void RenderView::init(RenderView::InitInfo& initInfo) {
    lug::Graphics::RenderView::init(initInfo);

    if (_info.renderTechniqueType == lug::Graphics::RenderTechnique::Type::Forward) {
        _renderTechnique = std::make_unique<ForwardRenderTechnique>(this);
    }
}

void RenderView::render() {
    if (_camera) {
        _camera->update(this);
        _renderTechnique->render(_camera->getRenderQueue());
    }
    else {
        LUG_LOG.warn("RenderView: Attempt to render with no camera attached");
    }
}

} // Vulkan
} // Graphics
} // lug
