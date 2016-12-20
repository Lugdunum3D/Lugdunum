#include <iterator>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {

Graphics::Graphics(const Core::Application::Info& appInfo) : _appInfo(appInfo) {}

void Graphics::init() {
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

} // Graphics
} // lug
