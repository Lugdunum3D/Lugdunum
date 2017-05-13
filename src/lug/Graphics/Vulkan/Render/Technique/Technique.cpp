#include <lug/Graphics/Vulkan/Render/Technique/Technique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace Technique {

Technique::Technique(const Renderer& renderer, const Render::View* renderView, const API::Device* device) :
    _renderer(renderer), _renderView{renderView}, _device{device} {}

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug
