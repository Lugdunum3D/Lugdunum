#include <lug/Graphics/Vulkan/Render/Technique/Technique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace Technique {

Technique::Technique(const Renderer& renderer, const Render::View* renderView, const API::Device* device, API::Queue* presentQueue) :
    _renderer(renderer), _renderView{renderView}, _device{device}, _presentQueue{presentQueue} {}

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug
