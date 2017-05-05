#include <lug/Graphics/Vulkan/Render/Material.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Material::Material(const std::string& name) : ::lug::Graphics::Render::Material(name) {

}

Material::~Material() {
    destroy();
}

void Material::destroy() {
    // TODO(Nokitoo): free device memory and buffer
}

} // Render
} // Vulkan
} // Graphics
} // lug
