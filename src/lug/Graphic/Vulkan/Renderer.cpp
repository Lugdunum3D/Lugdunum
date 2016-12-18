#include <lug/Graphic/Vulkan/Renderer.hpp>
#include <lug/Graphic/Graphic.hpp>
#include <lug/Graphic/Vulkan/Loader.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {

const std::unordered_map<Module::Type, Renderer::Requirements> Renderer::modulesRequirements = {
    #define LUG_INIT_GRAPHIC_MODULES_REQUIREMENTS(name) \
    {                                                   \
        Module::Type::name, name##Requirements          \
    },

    LUG_GRAPHIC_MODULES(LUG_INIT_GRAPHIC_MODULES_REQUIREMENTS)

    #undef LUG_INIT_GRAPHIC_MODULES_REQUIREMENTS
};

Renderer::Renderer(Graphic& graphic) : _graphic(graphic) {}

std::set<Module::Type> Renderer::init() {
    std::set<Module::Type> loadedModules{};

    // Load instance properties
    {
        // Load instance extensions
        {
            uint32_t extensionsCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr); // TODO: Check error

            _instanceInfo.extensions.resize(extensionsCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, &(_instanceInfo.extensions[0])); // TODO: Check error
        }

        // Load instance layers
        {
            uint32_t layersCount = 0;
            vkEnumerateInstanceLayerProperties(&layersCount, nullptr); // TODO: Check error

            _instanceInfo.layers.resize(layersCount);
            vkEnumerateInstanceLayerProperties(&layersCount, &(_instanceInfo.layers[0])); // TODO: Check error
        }
    }

    // Create instance
    {
        // Check which layers / extensions to load for modules
        checkRequirementsInstance(_graphic.getMandatoryModules(), loadedModules, false);
        checkRequirementsInstance(_graphic.getOptionnalModules(), loadedModules, true);

        // Create the application information for vkCreateInstance
        VkApplicationInfo applicationInfo{
            applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            applicationInfo.pNext = nullptr,
            applicationInfo.pApplicationName = _graphic.getAppInfo().name,
            applicationInfo.applicationVersion = _graphic.getAppInfo().version,
            applicationInfo.pEngineName = "Lugdunum3D",
            applicationInfo.engineVersion = VK_MAKE_VERSION(LUG_VERSION_MAJOR, LUG_VERSION_MINOR, LUG_VERSION_PATCH),
            applicationInfo.apiVersion = 0
        };

        // Create the instance creation information for vkCreateInstance
        VkInstanceCreateInfo createInfo{
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            createInfo.pNext = nullptr,
            createInfo.flags = 0,
            createInfo.pApplicationInfo = &applicationInfo,
            createInfo.enabledLayerCount = static_cast<uint32_t>(_loadedLayers.size()),
            createInfo.ppEnabledLayerNames = &_loadedLayers[0],
            createInfo.enabledExtensionCount = static_cast<uint32_t>(_loadedExtensions.size()),
            createInfo.ppEnabledExtensionNames = &_loadedExtensions[0]
        };

        VkInstance instance{VK_NULL_HANDLE};
        vkCreateInstance(&createInfo, nullptr, &instance); // TODO: Check error

        _instance = Instance(instance);
        _loader.loadInstanceFunctions(_instance);
    }

    return loadedModules;
}

void Renderer::checkRequirementsInstance(const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules, bool optionnal) {
    for (const auto moduleType : modulesToCheck) {
        const auto& requirements = modulesRequirements.at(moduleType);

        std::vector<const char*> layers;
        std::vector<const char*> extensions;

        for (const auto& layerName : requirements.mandatoryInstanceLayers) {
            if (_instanceInfo.containsLayer(layerName)) {
                layers.push_back(layerName);
            } else {
                if (!optionnal) {
                    // TODO: Handle error
                }

                break;
            }
        }

        for (const auto& extensionName : requirements.mandatoryInstanceExtensions) {
            if (_instanceInfo.containsExtension(extensionName)) {
                extensions.push_back(extensionName);
            } else {
                if (!optionnal) {
                    // TODO: Handle error
                }

                break;
            }
        }

        for (const auto& layerName : requirements.optionnalInstanceLayers) {
            if (_instanceInfo.containsLayer(layerName)) {
                layers.push_back(layerName);
            }
        }

        for (const auto& extensionName : requirements.optionnalInstanceExtensions) {
            if (_instanceInfo.containsExtension(extensionName)) {
                extensions.push_back(extensionName);
            }
        }

        loadedModules.insert(moduleType);

        _loadedLayers.insert(_loadedLayers.end(), layers.begin(), layers.end());
        _loadedExtensions.insert(_loadedExtensions.end(), extensions.begin(), extensions.end());
    }
}

} // Vulkan
} // Graphic
} // lug
