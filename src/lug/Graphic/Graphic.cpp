#include <iterator>
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

    _loadedModules = _renderer->init();

    // Check if all mandatory modules are loaded
    {
        std::set<Module::Type> intersect{};
        std::set_intersection(_loadedModules.begin(), _loadedModules.end(), _mandatoryModules.begin(), _mandatoryModules.end(), std::inserter(intersect,intersect.begin()));

        if (intersect.size() != _mandatoryModules.size()) {
            // TODO: Handle error (haven't load all the mandatory modules)
        }
    }
}

} // Graphic
} // lug
