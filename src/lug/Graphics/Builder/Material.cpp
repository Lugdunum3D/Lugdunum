#include <lug/Graphics/Builder/Material.hpp>

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Material::Material(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<Render::Material> Material::build() {
    switch (_renderer.getType()) {
        case Renderer::Type::Vulkan:
            return lug::Graphics::Vulkan::Builder::Material::build(*this);
    }

    return nullptr;
}

} // Builder
} // Graphics
} // lug
