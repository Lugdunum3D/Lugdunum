#include <lug/Graphics/Builder/Texture.hpp>

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Texture::Texture(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<Render::Texture> Texture::build() {
    Resource::SharedPtr<Render::Texture> texture{nullptr};

    switch (_renderer.getType()) {
        case Renderer::Type::Vulkan:
            texture = lug::Graphics::Vulkan::Builder::Texture::build(*this);
    }

    if (texture) {
        texture->_magFilter = _magFilter;
        texture->_minFilter = _minFilter;
        texture->_mipMapFilter = _mipMapFilter;
        texture->_wrapS = _wrapS;
        texture->_wrapT = _wrapT;
    }

    return texture;
}

} // Builder
} // Graphics
} // lug
