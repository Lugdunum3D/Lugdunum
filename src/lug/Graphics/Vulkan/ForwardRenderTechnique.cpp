#include <lug/Graphics/Vulkan/ForwardRenderTechnique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

ForwardRenderTechnique::ForwardRenderTechnique(const RenderView* renderView) : RenderTechnique(renderView) {}

void ForwardRenderTechnique::render(const RenderQueue& renderQueue) {
    // TODO: Render
    (void)(renderQueue);
}

} // Vulkan
} // Graphics
} // lug
