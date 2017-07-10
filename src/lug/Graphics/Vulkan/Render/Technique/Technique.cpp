#include <lug/Graphics/Vulkan/Render/Technique/Technique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace Technique {

Technique::Technique(Renderer& renderer, const Render::View& renderView) :
    _renderer(renderer), _renderView{renderView} {}

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug
