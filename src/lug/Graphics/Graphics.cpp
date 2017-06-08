#include <lug/Graphics/Graphics.hpp>

#include <iterator>
#include <sstream>
#include <map>

#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Vulkan/Render/Camera.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {

Graphics::Graphics(const std::string& appName, const Core::Version& appVersion) : _appName{appName}, _appVersion{appVersion} {}

bool Graphics::init(const InitInfo& initInfo) {
    return beginInit(initInfo) && finishInit();
}

bool Graphics::beginInit(const InitInfo& initInfo) {
    _initInfo = initInfo;

    switch (_initInfo.rendererType) {
        case Renderer::Type::Vulkan:
            _renderer = std::make_unique<Vulkan::Renderer>(*this);
            break;
        default:
            LUG_LOG.error("Graphics: Can't init renderer with specified render type");
            break;
    }

    _loadedMandatoryModules = _initInfo.mandatoryModules;
    _loadedOptionalModules = _initInfo.optionalModules;

    if (!_renderer->beginInit(_appName, _appVersion, _initInfo.rendererInitInfo)) {
        return false;
    }

    return true;
}

bool Graphics::finishInit() {
    if (!_renderer->finishInit()) {
        return false;
    }

    // Check if all mandatory modules are loaded
    {
        if (_loadedMandatoryModules.size() != _initInfo.mandatoryModules.size()) {
            LUG_LOG.error("Graphics: Can't init the engine with all the mandatory modules");
            return false;
        }
    }

#if defined(LUG_DEBUG)
    LUG_LOG.info("Graphics: Successfully init");

    {
        std::stringstream ss{};
        std::copy(_loadedMandatoryModules.begin(), _loadedMandatoryModules.end(), std::ostream_iterator<Module::Type>(ss, " "));
        LUG_LOG.info("Graphics: Mandatory modules loaded : {}", ss.str());
    }

    {
        std::stringstream ss{};
        std::copy(_loadedOptionalModules.begin(), _loadedOptionalModules.end(), std::ostream_iterator<Module::Type>(ss, " "));
        LUG_LOG.info("Graphics: Optional modules loaded : {}", ss.str());
    }
#endif

    return true;
}

void Graphics::unsupportedModule(Module::Type type) {
    {
        const auto it = std::find(std::begin(_loadedMandatoryModules), std::end(_loadedMandatoryModules), type);

        if (it != std::end(_loadedMandatoryModules)) {
            _loadedMandatoryModules.erase(it);
        }
    }

    {
        const auto it = std::find(std::begin(_loadedOptionalModules), std::end(_loadedOptionalModules), type);

        if (it != std::end(_loadedOptionalModules)) {
            _loadedOptionalModules.erase(it);
        }
    }
}

std::unique_ptr<Render::Camera> Graphics::createCamera(const std::string& name) {
    if (!_renderer) {
        LUG_LOG.error("Graphics: Can't create a camera, the renderer is not initialized");
        return nullptr;
    }

    std::unique_ptr<Render::Camera> camera = nullptr;

    if (_initInfo.rendererType == Renderer::Type::Vulkan) {
        camera = std::make_unique<Vulkan::Render::Camera>(name);
    } else {
        LUG_LOG.error("Graphics: Unknown render type");
    }

    return camera;
}

} // Graphics
} // lug
