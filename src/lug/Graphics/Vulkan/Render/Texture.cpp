#include <lug/Graphics/Vulkan/Render/Texture.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Texture::Texture(const std::string& name) : ::lug::Graphics::Render::Texture(name) {}

Texture::~Texture() {
    destroy();
}

void Texture::destroy() {
    _sampler.destroy();
    _imageView.destroy();
    _image.destroy();
}

} // Render
} // Vulkan
} // Graphics
} // lug
