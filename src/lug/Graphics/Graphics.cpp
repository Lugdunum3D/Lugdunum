#include <iterator>
#include <sstream>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/ModelLoader.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Mesh.hpp>
#include <lug/Graphics/Vulkan/Model.hpp>
#include <lug/Graphics/Vulkan/Camera.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {

Graphics::Graphics(const char* appName, uint32_t appVersion) : _appName{appName}, _appVersion{appVersion} {}

bool Graphics::init(const InitInfo& initInfo) {
    return beginInit(initInfo) && finishInit();
}

bool Graphics::beginInit(const InitInfo& initInfo) {
    _initInfo = initInfo;

    switch(_initInfo.rendererType) {
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

std::unique_ptr<Scene> Graphics::createScene() {
    return std::make_unique<Scene>();
}

std::unique_ptr<Mesh> Graphics::createMesh(const std::string& name) {
    if (!_renderer) {
        LUG_LOG.error("Graphics: Can't create a mesh, the renderer is not initialized");
        return nullptr;
    }

    std::unique_ptr<Mesh> mesh = nullptr;

    if (_initInfo.rendererType == Renderer::Type::Vulkan) {
        Vulkan::Renderer* renderer = static_cast<Vulkan::Renderer*>(_renderer.get());
        std::vector<uint32_t> queueFamilyIndices = { (uint32_t)renderer->getQueue(0, true)->getFamilyIdx() };
        mesh = std::make_unique<Vulkan::Mesh>(name, queueFamilyIndices, &renderer->getDevice());
    }
    else {
        LUG_LOG.error("Graphics: Unknown render type");
    }

    return mesh;
}

std::unique_ptr<Model> Graphics::createModel(const std::string& name, const std::string& fileName) {
    if (!_renderer) {
        LUG_LOG.error("Graphics: Can't create a model, the renderer is not initialized");
        return nullptr;
    }

    std::unique_ptr<Model> model = nullptr;

    if (_initInfo.rendererType == Renderer::Type::Vulkan) {
        Vulkan::Renderer* renderer = static_cast<Vulkan::Renderer*>(_renderer.get());
        std::vector<uint32_t> queueFamilyIndices = { (uint32_t)renderer->getQueue(0, true)->getFamilyIdx() };
        model = std::make_unique<Vulkan::Model>(name, queueFamilyIndices, &renderer->getDevice());
    }
    else {
        LUG_LOG.error("Graphics: Unknown render type");
        return nullptr;
    }

    if (fileName.size() != 0) {
        if (!ModelLoader::loadFromFile(model.get(), fileName) ||
            !model->load()) {
            return nullptr;
        }
    }


    return model;
}

std::unique_ptr<Camera> Graphics::createCamera(const std::string& name) {
    if (!_renderer) {
        LUG_LOG.error("Graphics: Can't create a camera, the renderer is not initialized");
        return nullptr;
    }

    std::unique_ptr<Camera> camera = nullptr;

    if (_initInfo.rendererType == Renderer::Type::Vulkan) {
        camera = std::make_unique<Vulkan::Camera>(name);
    }
    else {
        LUG_LOG.error("Graphics: Unknown render type");
    }

    return camera;
}

} // Graphics
} // lug
