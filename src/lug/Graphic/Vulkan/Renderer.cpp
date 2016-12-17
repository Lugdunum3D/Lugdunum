#include <lug/Graphic/Vulkan/Renderer.hpp>
#include <lug/Graphic/Graphic.hpp>
#include <lug/Graphic/Vulkan/Loader.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {

Renderer::Renderer(Graphic& graphic) : _graphic(graphic) {}

void Renderer::init() {
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
        std::vector<const char*> layers;
        std::vector<const char*> extensions;

        // Check which layers / extensions to load
        for (const auto moduleType : _graphic.getModulesLoaded()) {
            const auto& module = ::lug::Graphic::modules.at(moduleType);

            for (const auto& layerName : module.requirements.mandatoryInstanceLayers) {
                if (_instanceInfo.containsLayer(layerName)) {
                    layers.push_back(layerName);
                } else {
                    // TODO: Handle error
                }
            }

            for (const auto& extensionName : module.requirements.mandatoryInstanceExtensions) {
                if (_instanceInfo.containsExtension(extensionName)) {
                    extensions.push_back(extensionName);
                } else {
                    // TODO: Handle error
                }
            }

            for (const auto& layerName : module.requirements.optionnalInstanceLayers) {
                if (_instanceInfo.containsLayer(layerName)) {
                    layers.push_back(layerName);
                }
            }

            for (const auto& extensionName : module.requirements.optionnalInstanceExtensions) {
                if (_instanceInfo.containsExtension(extensionName)) {
                    extensions.push_back(extensionName);
                }
            }
        }

        // Create the application information for vkCreateInstance
        VkApplicationInfo applicationInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = _graphic.getAppInfo().name,
            .applicationVersion = _graphic.getAppInfo().version,
            .pEngineName = "Lugdunum3D",
            .engineVersion = VK_MAKE_VERSION(LUG_VERSION_MAJOR, LUG_VERSION_MINOR, LUG_VERSION_PATCH),
            .apiVersion = 0
        };

        // Create the instance creation information for vkCreateInstance
        VkInstanceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = &layers[0],
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = &extensions[0]
        };

        VkInstance instance{VK_NULL_HANDLE};
        vkCreateInstance(&createInfo, nullptr, &instance); // TODO: Check error

        _instance = Instance(instance);
        _loader.loadInstanceFunctions(_instance);
    }
}

} // Vulkan
} // Graphic
} // lug
