#include <iterator>
#include <sstream>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {

Graphics::Graphics(const Core::Application::Info& appInfo) : _appInfo(appInfo) {}

bool Graphics::init() {
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
            LUG_LOG.error("Graphics: Can't init the engine with all the mandatory modules");
            return false;
        }
    }

#if defined(LUG_DEBUG)
    LUG_LOG.info("Graphics: Successfully init");

    {
        std::stringstream ss{};
        std::copy(_loadedModules.begin(), _loadedModules.end(), std::ostream_iterator<Module::Type>(ss, " "));
        LUG_LOG.info("Graphics: Modules loaded : {}", ss.str());
    }
#endif

    return true;
}

} // Graphics
} // lug
