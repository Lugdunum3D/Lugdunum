#include <lug/Graphic/Vulkan/Renderer.hpp>
#include <lug/Graphic/Graphic.hpp>
#include <lug/Graphic/Vulkan/Loader.hpp>
#include <lug/Graphic/Vulkan/Requirements/Core.hpp>
#include <lug/Graphic/Vulkan/Requirements/Requirements.hpp>

#include <iostream> // TO REMOVE

namespace lug {
namespace Graphic {
namespace Vulkan {

const std::unordered_map<Module::Type, Renderer::Requirements> Renderer::modulesRequirements = {
    #define LUG_INIT_GRAPHIC_MODULES_REQUIREMENTS(name)                                 \
    {                                                                                   \
        Module::Type::name, ::lug::Graphic::Vulkan::Requirements::name::requirements    \
    },

    LUG_GRAPHIC_MODULES(LUG_INIT_GRAPHIC_MODULES_REQUIREMENTS)

    #undef LUG_INIT_GRAPHIC_MODULES_REQUIREMENTS
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(
    VkDebugReportFlagsEXT /*flags*/,
    VkDebugReportObjectTypeEXT /*objType*/,
    uint64_t /*obj*/,
    size_t /*location*/,
    int32_t /*code*/,
    const char* /*layerPrefix*/,
    const char* msg,
    void* /*userData*/) {

    // TODO: Log informations
    std::cerr << "Validation layer: " << msg << std::endl;

    return VK_FALSE;
}

Renderer::Renderer(Graphic& graphic) : _graphic(graphic) {}

Renderer::~Renderer() {
    for (auto& queue : _queues) {
        queue.destroy();
    }

    _device.destroy();

    // Destroy the report callback if necessary
    {
        if (isInstanceExtensionLoaded(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
            auto vkDestroyDebugReportCallbackEXT = _instance.getProcAddr<PFN_vkDestroyDebugReportCallbackEXT>("vkDestroyDebugReportCallbackEXT");
            vkDestroyDebugReportCallbackEXT(_instance, _debugReportCallback, nullptr); // TODO: Check error
        }
    }

    _instance.destroy();
}

std::set<Module::Type> Renderer::init() {
    std::set<Module::Type> loadedModules;

    loadedModules.insert(_graphic.getMandatoryModules().begin(), _graphic.getMandatoryModules().end());
    loadedModules.insert(_graphic.getOptionnalModules().begin(), _graphic.getOptionnalModules().end());

    if (!initInstance(loadedModules)) {
        // TODO: Log something
        return {};
    }

    if (!initDevice(loadedModules)) {
        // TODO: Log something
        return {};
    }

    return loadedModules;
}

bool Renderer::initInstance(std::set<Module::Type>& loadedModules) {
    // Load instance properties
    {
        // Load instance extensions
        {
            uint32_t extensionsCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr); // TODO: Check error

            _instanceInfo.extensions.resize(extensionsCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, _instanceInfo.extensions.data()); // TODO: Check error
        }

        // Load instance layers
        {
            uint32_t layersCount = 0;
            vkEnumerateInstanceLayerProperties(&layersCount, nullptr); // TODO: Check error

            _instanceInfo.layers.resize(layersCount);
            vkEnumerateInstanceLayerProperties(&layersCount, _instanceInfo.layers.data()); // TODO: Check error
        }
    }

    // Create instance
    {
        // Check which layers / extensions to load for modules
        if (!checkRequirementsInstance(_graphic.getMandatoryModules(), loadedModules, false) || !checkRequirementsInstance(_graphic.getOptionnalModules(), loadedModules, true)) {
            return false;
        }

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
            createInfo.enabledLayerCount = static_cast<uint32_t>(_loadedInstanceLayers.size()),
            createInfo.ppEnabledLayerNames = _loadedInstanceLayers.data(),
            createInfo.enabledExtensionCount = static_cast<uint32_t>(_loadedInstanceExtensions.size()),
            createInfo.ppEnabledExtensionNames = _loadedInstanceExtensions.data()
        };

        VkInstance instance{VK_NULL_HANDLE};
        vkCreateInstance(&createInfo, nullptr, &instance); // TODO: Check error

        _instance = Instance(instance);
        _loader.loadInstanceFunctions(_instance);
    }

    // Create report callback if necessary
    {
        if (isInstanceExtensionLoaded(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
            VkDebugReportCallbackCreateInfoEXT createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
            createInfo.pfnCallback = debugReportCallback;

            auto vkCreateDebugReportCallbackEXT = _instance.getProcAddr<PFN_vkCreateDebugReportCallbackEXT>("vkCreateDebugReportCallbackEXT");
            vkCreateDebugReportCallbackEXT(_instance, &createInfo, nullptr, &_debugReportCallback); // TODO: Check error
        }
    }

    // Load physical devices information
    {
        uint32_t physicalDevicesCount = 0;
        vkEnumeratePhysicalDevices(_instance, &physicalDevicesCount, nullptr); // TODO: Check error

        _physicalDeviceInfos.resize(physicalDevicesCount);

        std::vector<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
        vkEnumeratePhysicalDevices(_instance, &physicalDevicesCount, physicalDevices.data()); // TODO: Check error

        for (uint8_t idx = 0; idx < physicalDevicesCount; ++idx) {
            _physicalDeviceInfos[idx].handle = physicalDevices[idx];

            vkGetPhysicalDeviceProperties(physicalDevices[idx], &(_physicalDeviceInfos[idx].properties)); // TODO: Check error
            vkGetPhysicalDeviceFeatures(physicalDevices[idx], &(_physicalDeviceInfos[idx].features)); // TODO: Check error

            // Load queue families
            {
                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[idx], &queueFamilyCount, nullptr); // TODO: Check error

                _physicalDeviceInfos[idx].queueFamilies.resize(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[idx], &queueFamilyCount, _physicalDeviceInfos[idx].queueFamilies.data()); // TODO: Check error
            }

            vkGetPhysicalDeviceMemoryProperties(physicalDevices[idx], &(_physicalDeviceInfos[idx].memoryProperties)); // TODO: Check error

            // Load device extensions
            {
                uint32_t extensionsCount = 0;
                vkEnumerateDeviceExtensionProperties(physicalDevices[idx], nullptr, &extensionsCount, nullptr); // TODO: Check error

                _physicalDeviceInfos[idx].extensions.resize(extensionsCount);
                vkEnumerateDeviceExtensionProperties(physicalDevices[idx], nullptr, &extensionsCount, _physicalDeviceInfos[idx].extensions.data()); // TODO: Check error
            }

            // TODO: Get additionnal informations (images properties, etc)
        }
    }

    return true;
}

bool Renderer::checkRequirementsInstance(const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules, bool optionnal) {
    bool requirementsCheck = true;

    for (const auto moduleType : modulesToCheck) {
        const auto& module = modules.at(moduleType);
        const auto& requirements = modulesRequirements.at(moduleType);

        std::vector<const char*> layers{};
        std::vector<const char*> extensions{};

        bool moduleRequirementsCheck = true;

        for (const auto& layerName : requirements.mandatoryInstanceLayers) {
            if (_instanceInfo.containsLayer(layerName)) {
                layers.push_back(layerName);
            } else {
                if (!optionnal) {
                    // TODO: Log error
                    std::cout << "Error: Can't load mandatory layer '" << layerName << "' for mandatory module '" << module.name << "'" << std::endl;
                } else {
                    // TODO: Log error
                    std::cout << "Warning: Can't load mandatory layer '" << layerName << "' for optionnal module '" << module.name << "'" << std::endl;
                }

                moduleRequirementsCheck = false;
            }
        }

        for (const auto& extensionName : requirements.mandatoryInstanceExtensions) {
            if (_instanceInfo.containsExtension(extensionName)) {
                extensions.push_back(extensionName);
            } else {
                if (!optionnal) {
                    // TODO: Log error
                    std::cout << "Error: Can't load mandatory extension '" << extensionName << "' for mandatory module '" << module.name << "'" << std::endl;
                } else {
                    // TODO: Log error
                    std::cout << "Warning: Can't load mandatory extension '" << extensionName << "' for optionnal module '" << module.name << "'" << std::endl;
                }

                moduleRequirementsCheck = false;
            }
        }

        for (const auto& layerName : requirements.optionnalInstanceLayers) {
            if (_instanceInfo.containsLayer(layerName)) {
                layers.push_back(layerName);
            } else {
                // TODO: Log error
                std::cout << "Warning: Can't load optionnal layer '" << layerName << "' for module '" << module.name << "'" << std::endl;
            }
        }

        for (const auto& extensionName : requirements.optionnalInstanceExtensions) {
            if (_instanceInfo.containsExtension(extensionName)) {
                extensions.push_back(extensionName);
            } else {
                // TODO: Log error
                std::cout << "Warning: Can't load optionnal extension '" << extensionName << "' for module '" << module.name << "'" << std::endl;
            }
        }

        if (moduleRequirementsCheck) {
            _loadedInstanceLayers.insert(_loadedInstanceLayers.end(), layers.begin(), layers.end());
            _loadedInstanceExtensions.insert(_loadedInstanceExtensions.end(), extensions.begin(), extensions.end());
        } else {
            loadedModules.erase(moduleType);
        }

        if (!optionnal) {
            requirementsCheck = requirementsCheck && moduleRequirementsCheck;
        }
    }

    return requirementsCheck;
}

bool Renderer::initDevice(std::set<Module::Type> &loadedModules) {
    // Select device
    {
        uint8_t matchedDeviceIdx = 0;
        std::vector<uint8_t> matchedDevicesIdx{};

        for (uint8_t idx = 0; idx < _physicalDeviceInfos.size(); ++idx) {
            std::set<Module::Type> tmpLoadedModules;
            if (!checkRequirementsDevice(_physicalDeviceInfos[idx], _graphic.getMandatoryModules(), tmpLoadedModules, false, false)) {
                continue;
            }

            if (!checkRequirementsDevice(_physicalDeviceInfos[idx], _graphic.getOptionnalModules(), tmpLoadedModules, true, false)) {
                continue;
            }

            matchedDevicesIdx.push_back(idx);
        }

        if (matchedDevicesIdx.size() == 0) {
            // TODO: Log error
            return false;
        }

        // TODO: Add score
        matchedDeviceIdx = matchedDevicesIdx[0];
        for (uint8_t idx : matchedDevicesIdx) {
            if (_physicalDeviceInfos[idx].properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                matchedDeviceIdx = idx;
                break;
            }
        }

        _loadedDeviceExtensions.clear();
        _loadedDeviceFeatures = VkPhysicalDeviceFeatures{};
        _loadedQueueFamiliesIdx.clear();

        _physicalDeviceInfo = &(_physicalDeviceInfos[matchedDeviceIdx]);
    }

    // Set the loaded informations of the matched device
    {
        checkRequirementsDevice(*_physicalDeviceInfo, _graphic.getMandatoryModules(), loadedModules, false, true);
        checkRequirementsDevice(*_physicalDeviceInfo, _graphic.getOptionnalModules(), loadedModules, true, true);
    }

    // Create device
    {
        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfo(_loadedQueueFamiliesIdx.size());

        {
            uint8_t i = 0;

            for (auto idx : _loadedQueueFamiliesIdx) {
                queueCreateInfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo[i].queueFamilyIndex = idx;
                queueCreateInfo[i].queueCount = 1;
                queueCreateInfo[i].pQueuePriorities = &queuePriority;

                ++i;
            }
        }

        VkDeviceCreateInfo createInfo{
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            createInfo.pNext = nullptr,
            createInfo.flags = 0,
            createInfo.queueCreateInfoCount = queueCreateInfo.size(),
            createInfo.pQueueCreateInfos = queueCreateInfo.data(),
            createInfo.enabledLayerCount = 0, // Deprecated
            createInfo.ppEnabledLayerNames = nullptr, // Deprecated
            createInfo.enabledExtensionCount = static_cast<uint32_t>(_loadedDeviceExtensions.size()),
            createInfo.ppEnabledExtensionNames = _loadedDeviceExtensions.data(),
            createInfo.pEnabledFeatures = &_loadedDeviceFeatures
        };

        VkDevice device{VK_NULL_HANDLE};
        vkCreateDevice(_physicalDeviceInfo->handle, &createInfo, nullptr, &device); // TODO: Check error

        _device = Device(device);
        _loader.loadDeviceFunctions(_device);
    }

    // Create queues
    {
        _queues.resize(_loadedQueueFamiliesIdx.size());

        uint8_t i = 0;
        for (auto idx : _loadedQueueFamiliesIdx) {
            VkQueue queue;
            vkGetDeviceQueue(_device, idx, 0, &queue); // TODO: Check error

            _queues[i] = Queue(idx, queue, _physicalDeviceInfo->queueFamilies[idx].queueFlags);

            ++i;
        }
    }

    return true;
}

bool Renderer::checkRequirementsDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules, bool optionnal, bool quiet) {
    bool requirementsCheck = true;

    for (const auto moduleType : modulesToCheck) {
        const auto& module = modules.at(moduleType);
        const auto& requirements = modulesRequirements.at(moduleType);

        std::vector<const char*> extensions{};
        VkPhysicalDeviceFeatures features{};
        std::set<int8_t> queueFamiliesIdx{};

        bool moduleRequirementsCheck = true;

        for (const auto& extensionName : requirements.mandatoryDeviceExtensions) {
            if (physicalDeviceInfo.containsExtension(extensionName)) {
                extensions.push_back(extensionName);
            } else if (!quiet) {
                if (!optionnal) {
                    // TODO: Log error
                    std::cout << "Error: Can't load mandatory extension '" << extensionName << "' for mandatory module '" << module.name << "'" << std::endl;
                } else {
                    // TODO: Log error
                    std::cout << "Warning: Can't load mandatory extension '" << extensionName << "' for optionnal module '" << module.name << "'" << std::endl;
                }

                moduleRequirementsCheck = false;
            }
        }

        for (const auto& extensionName : requirements.optionnalDeviceExtensions) {
            if (physicalDeviceInfo.containsExtension(extensionName)) {
                extensions.push_back(extensionName);
            } else if (!quiet) {
                // TODO: Log error
                std::cout << "Warning: Can't load optionnal extension '" << extensionName << "' for module '" << module.name << "'" << std::endl;
            }
        }

        // TODO: Log error
        #define LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES(featureName)                                                                            \
            {                                                                                                                                               \
                if (requirements.mandatoryFeatures.featureName == VK_TRUE) {                                                                                \
                    if (physicalDeviceInfo.features.featureName == VK_TRUE) {                                                                               \
                        features.featureName = VK_TRUE;                                                                                                     \
                    } else if (!quiet) {                                                                                                                    \
                        if (!optionnal) {                                                                                                                   \
                            std::cout << "Error: Can't load mandatory feature '" << #featureName << "' for module '" << module.name << "'" << std::endl;    \
                        } else {                                                                                                                            \
                            std::cout << "Warning: Can't load mandatory feature '" << #featureName << "' for module '" << module.name << "'" << std::endl;  \
                        }                                                                                                                                   \
                                                                                                                                                            \
                        moduleRequirementsCheck = false;                                                                                                    \
                    }                                                                                                                                       \
                }                                                                                                                                           \
            }
        LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES);
        #undef LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES

        // TODO: Log error
        #define LUG_CHECK_VULKAN_PHYSICAL_DEVICE_OPTIONNAL_FEATURES(featureName)                                                                        \
            {                                                                                                                                           \
                if (requirements.optionnalFeatures.featureName == VK_TRUE) {                                                                            \
                    if (physicalDeviceInfo.features.featureName == VK_TRUE) {                                                                           \
                        features.featureName = VK_TRUE;                                                                                                 \
                    } else if (!quiet) {                                                                                                                \
                        std::cout << "Warning: Can't load optionnal feature '" << #featureName << "' for module '" << module.name << "'" << std::endl;  \
                    }                                                                                                                                   \
                }                                                                                                                                       \
            }
        LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_CHECK_VULKAN_PHYSICAL_DEVICE_OPTIONNAL_FEATURES);
        #undef LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES

        for (const auto& queueFlags : requirements.mandatoryQueueFlags) {
            int8_t idx = 0;
            if (physicalDeviceInfo.containsQueueFlags(queueFlags, idx)) {
                queueFamiliesIdx.insert(idx);
            } else if (!quiet) {
                if (!optionnal) {
                    std::cout << "Error: Can't find mandatory queue type for module '" << module.name << "'" << std::endl;
                } else {
                    std::cout << "Warning: Can't find mandatory queue type for module '" << module.name << "'" << std::endl;
                }

                moduleRequirementsCheck = false;
                break;
            }
        }

        for (const auto& queueFlags : requirements.optionnalQueueFlags) {
            int8_t idx = 0;
            if (physicalDeviceInfo.containsQueueFlags(queueFlags, idx)) {
                queueFamiliesIdx.insert(idx);
            } else if (!quiet) {
                std::cout << "Warning: Can't find optionnal queue type for module '" << module.name << "'" << std::endl;
            }
        }

        if (moduleRequirementsCheck) {
            _loadedDeviceExtensions.insert(_loadedDeviceExtensions.end(), extensions.begin(), extensions.end());

            #define LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES(featureName) _loadedDeviceFeatures.featureName = _loadedDeviceFeatures.featureName || features.featureName;
            LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES);
            #undef LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES

            _loadedQueueFamiliesIdx.insert(queueFamiliesIdx.begin(), queueFamiliesIdx.end());
        } else {
            loadedModules.erase(moduleType);
        }

        if (!optionnal) {
            requirementsCheck = requirementsCheck && moduleRequirementsCheck;
        }
    }

    return requirementsCheck;
}

} // Vulkan
} // Graphic
} // lug
