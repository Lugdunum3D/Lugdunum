#include <lug/Graphics/Builder/SkyBox.hpp>

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

SkyBox::SkyBox(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<Render::SkyBox> SkyBox::build() {
    switch (_renderer.getType()) {
        case Renderer::Type::Vulkan:
            return lug::Graphics::Vulkan::Builder::SkyBox::build(*this);
    }

    return nullptr;
}

} // Builder
} // Graphics
} // lug
