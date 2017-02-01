#include <lug/Graphics/Vulkan/RenderTechnique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

RenderTechnique::RenderTechnique(const RenderView* renderView, const Device* device, Queue* presentQueue) :
                                _renderView{renderView}, _device{device}, _presentQueue{presentQueue} {}

} // Vulkan
} // Graphics
} // lug
