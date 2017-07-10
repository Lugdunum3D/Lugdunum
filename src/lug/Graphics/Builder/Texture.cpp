#include <lug/Graphics/Builder/Texture.hpp>

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Texture::Texture(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<Render::Texture> Texture::build() {
    switch (_renderer.getType()) {
        case Renderer::Type::Vulkan:
            return lug::Graphics::Vulkan::Builder::Texture::build(*this);
    }

    return nullptr;
}

} // Builder
} // Graphics
} // lug
