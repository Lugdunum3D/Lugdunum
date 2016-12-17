#include <lug/Graphic/Graphic.hpp>
#include <lug/Graphic/Module.hpp>
#include <lug/Graphic/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphic {

Graphic::Graphic(const Core::Application::Info& appInfo) : _appInfo(appInfo) {}

void Graphic::init() {
    switch(_rendererType) {
        case Renderer::Type::Vulkan:
            _renderer = std::make_unique<Vulkan::Renderer>(*this);
            break;
    }

    _renderer->init();
}

} // Graphic
} // lug
