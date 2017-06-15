#include <lug/Graphics/Builder/Mesh.hpp>

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Mesh::Mesh(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<Render::Mesh> Mesh::build() {
    switch (_renderer.getType()) {
        case Renderer::Type::Vulkan:
            return lug::Graphics::Vulkan::Builder::Mesh::build(*this);
    }

    return nullptr;
}

} // Builder
} // Graphics
} // lug
