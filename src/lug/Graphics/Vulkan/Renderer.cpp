#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Vulkan/Loader.hpp>
#include <lug/Graphics/Vulkan/Requirements/Core.hpp>
#include <lug/Graphics/Vulkan/Requirements/Requirements.hpp>
#include <lug/Graphics/Vulkan/RenderWindow.hpp>
#include <lug/System/Logger.hpp>
#include <lug/Math/Geometry/Transform.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

const std::unordered_map<Module::Type, Renderer::Requirements> Renderer::modulesRequirements = {
    #define LUG_INIT_GRAPHICS_MODULES_REQUIREMENTS(name)                                 \
    {                                                                                   \
        Module::Type::name, ::lug::Graphics::Vulkan::Requirements::name::requirements    \
    },

    LUG_GRAPHICS_MODULES(LUG_INIT_GRAPHICS_MODULES_REQUIREMENTS)

    #undef LUG_INIT_GRAPHICS_MODULES_REQUIREMENTS
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT /*objType*/,
    uint64_t /*obj*/,
    size_t /*location*/,
    int32_t /*code*/,
    const char* layerPrefix,
    const char* msg,
    void* /*userData*/) {

    // Convert VkDebugReportFlagsEXT to System::Level
    System::Level level = System::Level::Off;
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        level = System::Level::Error;
    } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        level = System::Level::Warning;
    } else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        level = System::Level::Info;
    } else {
        level = System::Level::Debug;
    }

    LUG_LOG.log(level, "{}: {}", layerPrefix, msg);

    return VK_FALSE;
}

Renderer::~Renderer() {
    destroy();
}

void Renderer::destroy() {
    for (auto& queue : _queues) {
        queue.waitIdle();
    }

    // Destroy the window
    _window.reset();

    for (auto& queue : _queues) {
        queue.destroy();
    }

    _device.destroy();

    // Destroy the report callback if necessary
    {
        if (isInstanceExtensionLoaded(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
            auto vkDestroyDebugReportCallbackEXT = _instance.getProcAddr<PFN_vkDestroyDebugReportCallbackEXT>("vkDestroyDebugReportCallbackEXT");
            vkDestroyDebugReportCallbackEXT(_instance, _debugReportCallback, nullptr);
        }
    }

    _instance.destroy();
}

std::set<Module::Type> Renderer::init(const char* appName, uint32_t appVersion, const Renderer::InitInfo& initInfo) {
    std::set<Module::Type> loadedModules;

    loadedModules.insert(initInfo.mandatoryModules.begin(), initInfo.mandatoryModules.end());
    loadedModules.insert(initInfo.optionalModules.begin(), initInfo.optionalModules.end());

    if (!initInstance(appName, appVersion, initInfo, loadedModules)) {
        LUG_LOG.error("RendererVulkan: Can't load the instance");
        return {};
    }

    if (!initDevice(initInfo, loadedModules)) {
        LUG_LOG.error("RendererVulkan: Can't load the device");
        return {};
    }

#if defined(LUG_DEBUG)
    LUG_LOG.info("RendererVulkan: Use device {}", _physicalDeviceInfo->properties.deviceName);
#endif

    return loadedModules;
}

/**
 * Initialize the Vulkan instance
 * @param  loadedModules Modules to load
 * @return               Wether the instance was loaded successfully or not
 */
bool Renderer::initInstance(const char* appName, uint32_t appVersion, const Renderer::InitInfo& initInfo, std::set<Module::Type>& loadedModules) {
    VkResult result;

    // Load instance properties
    {
        // Load instance extensions
        {
            uint32_t extensionsCount = 0;
            result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't enumerate instance extensions: {}", result);
                return false;
            }

            _instanceInfo.extensions.resize(extensionsCount);
            result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, _instanceInfo.extensions.data());
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't enumerate instance extensions: {}", result);
                return false;
            }
        }

        // Load instance layers
        {
            uint32_t layersCount = 0;
            result = vkEnumerateInstanceLayerProperties(&layersCount, nullptr);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't enumerate instance layers: {}", result);
                return false;
            }

            _instanceInfo.layers.resize(layersCount);
            result = vkEnumerateInstanceLayerProperties(&layersCount, _instanceInfo.layers.data());
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't enumerate instance layers: {}", result);
                return false;
            }
        }
    }

    // Create instance
    {
        // Check which layers / extensions to load for modules
        if (!checkRequirementsInstance(initInfo.mandatoryModules, loadedModules)) {
            return false;
        }

        checkRequirementsInstance(initInfo.optionalModules, loadedModules);

        // Create the application information for vkCreateInstance
        VkApplicationInfo applicationInfo{
            applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            applicationInfo.pNext = nullptr,
            applicationInfo.pApplicationName = appName,
            applicationInfo.applicationVersion = appVersion,
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
        result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't create the instance: {}", result);
            return false;
        }

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
            vkCreateDebugReportCallbackEXT(_instance, &createInfo, nullptr, &_debugReportCallback);
        }
    }

    // Load physical devices information
    {
        uint32_t physicalDevicesCount = 0;
        result = vkEnumeratePhysicalDevices(_instance, &physicalDevicesCount, nullptr);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't enumerate physical devices: {}", result);
            return false;
        }

        _physicalDeviceInfos.resize(physicalDevicesCount);

        std::vector<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
        result = vkEnumeratePhysicalDevices(_instance, &physicalDevicesCount, physicalDevices.data());
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't enumerate physical devices: {}", result);
            return false;
        }

        for (uint8_t idx = 0; idx < physicalDevicesCount; ++idx) {
            _physicalDeviceInfos[idx].handle = physicalDevices[idx];

            vkGetPhysicalDeviceProperties(physicalDevices[idx], &(_physicalDeviceInfos[idx].properties));
            vkGetPhysicalDeviceFeatures(physicalDevices[idx], &(_physicalDeviceInfos[idx].features));

            // Load queue families
            {
                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[idx], &queueFamilyCount, nullptr);

                _physicalDeviceInfos[idx].queueFamilies.resize(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[idx], &queueFamilyCount, _physicalDeviceInfos[idx].queueFamilies.data());
            }

            vkGetPhysicalDeviceMemoryProperties(physicalDevices[idx], &(_physicalDeviceInfos[idx].memoryProperties));

            // Load device extensions
            {
                uint32_t extensionsCount = 0;
                result = vkEnumerateDeviceExtensionProperties(physicalDevices[idx], nullptr, &extensionsCount, nullptr);
                if (result != VK_SUCCESS) {
                    LUG_LOG.error("RendererVulkan: Can't enumerate device extensions: {}", result);
                    return false;
                }

                _physicalDeviceInfos[idx].extensions.resize(extensionsCount);
                result = vkEnumerateDeviceExtensionProperties(physicalDevices[idx], nullptr, &extensionsCount, _physicalDeviceInfos[idx].extensions.data());
                if (result != VK_SUCCESS) {
                    LUG_LOG.error("RendererVulkan: Can't enumerate device extensions: {}", result);
                    return false;
                }
            }

            // Load images formats properties
            // The VkFormat enum members are from 1 to 184
            {
                for (uint32_t i = 1; i <= 184; ++i) {
                    VkFormat format = (VkFormat)i;
                    vkGetPhysicalDeviceFormatProperties(physicalDevices[idx], format, &_physicalDeviceInfos[idx].formatProperties[format]);
                }

            }

            // TODO: Get additional informations (images properties, etc)
        }
    }

    return true;
}

/**
 * Initialize the Vulkan device
 * @param  loadedModules Modules to load
 * @return               Wether the device was loaded successfully or not
 */
bool Renderer::initDevice(const Renderer::InitInfo& initInfo, std::set<Module::Type> &loadedModules) {
    VkResult result;

    // Select device
    {
        uint8_t matchedDeviceIdx = 0;
        std::vector<uint8_t> matchedDevicesIdx{};

        for (uint8_t idx = 0; idx < _physicalDeviceInfos.size(); ++idx) {
            std::set<Module::Type> tmpLoadedModules;
            if (!checkRequirementsDevice(_physicalDeviceInfos[idx], initInfo.mandatoryModules, tmpLoadedModules, false)) {
                continue;
            }

            checkRequirementsDevice(_physicalDeviceInfos[idx], initInfo.optionalModules, tmpLoadedModules, false);

            matchedDevicesIdx.push_back(idx);
        }

        if (matchedDevicesIdx.size() == 0) {
            LUG_LOG.error("RendererVulkan: Can't find a compatible device");
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

        _physicalDeviceInfo = &(_physicalDeviceInfos[matchedDeviceIdx]);
    }

    // Set the loaded informations of the matched device
    {
        checkRequirementsDevice(*_physicalDeviceInfo, initInfo.mandatoryModules, loadedModules, true);
        checkRequirementsDevice(*_physicalDeviceInfo, initInfo.optionalModules, loadedModules, true);
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
            createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfo.size()),
            createInfo.pQueueCreateInfos = queueCreateInfo.data(),
            createInfo.enabledLayerCount = 0, // Deprecated
            createInfo.ppEnabledLayerNames = nullptr, // Deprecated
            createInfo.enabledExtensionCount = static_cast<uint32_t>(_loadedDeviceExtensions.size()),
            createInfo.ppEnabledExtensionNames = _loadedDeviceExtensions.data(),
            createInfo.pEnabledFeatures = &_loadedDeviceFeatures
        };

        VkDevice device{VK_NULL_HANDLE};
        result = vkCreateDevice(_physicalDeviceInfo->handle, &createInfo, nullptr, &device);
        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't create the device: {}", result);
            return false;
        }

        _device = Device(device, _physicalDeviceInfo);
        _loader.loadDeviceFunctions(_device);
    }

    // Create queues
    {
        _queues.resize(_loadedQueueFamiliesIdx.size());

        uint8_t i = 0;
        for (auto idx : _loadedQueueFamiliesIdx) {
            VkQueue queue;
            vkGetDeviceQueue(_device, idx, 0, &queue);

            _queues[i] = Queue(idx, queue, _physicalDeviceInfo->queueFamilies[idx].queueFlags);

            ++i;
        }
    }

    // Create command pools
    {
        for (auto& queue : _queues) {
            VkCommandPoolCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // TODO: Maybe change that
                createInfo.queueFamilyIndex = queue.getFamilyIdx()
            };

            VkCommandPool commandPool{VK_NULL_HANDLE};
            result = vkCreateCommandPool(_device, &createInfo, nullptr, &commandPool);
            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't create a command pool: {}", result);
                return false;
            }

            queue.setCommandPool(CommandPool(commandPool, &_device, &queue));
        }
    }
    return true;
}

bool Renderer::checkRequirementsInstance(const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules) {
    bool requirementsCheck = true;

    for (const auto moduleType : modulesToCheck) {
        const auto& requirements = modulesRequirements.at(moduleType);

        std::vector<const char*> layers{};
        std::vector<const char*> extensions{};

        bool moduleRequirementsCheck = true;

        // Check layers
        {
            const std::vector<const char*> layersNotFound = checkRequirementsLayers(_instanceInfo, requirements.mandatoryInstanceLayers, layers);
            moduleRequirementsCheck = moduleRequirementsCheck && layersNotFound.size() == 0;

            for (const char* const layerName : layersNotFound) {
                LUG_LOG.warn("Can't load mandatory layer '{}' for module '{}'", layerName, moduleType);
            }
        }

        {
            const std::vector<const char*> layersNotFound = checkRequirementsLayers(_instanceInfo, requirements.optionalInstanceLayers, layers);

            for (const char* layerName : layersNotFound) {
                LUG_LOG.warn("Can't load optional layer '{}' for module '{}'", layerName, moduleType);
            }
        }

        // Check extensions
        {
            const std::vector<const char*> extensionsNotFound = checkRequirementsExtensions(_instanceInfo, requirements.mandatoryInstanceExtensions, extensions);
            moduleRequirementsCheck = moduleRequirementsCheck && extensionsNotFound.size() == 0;

            for (const char* const extensionName : extensionsNotFound) {
                LUG_LOG.warn("Can't load mandatory extension '{}' for module '{}'", extensionName, moduleType);
            }
        }

        {
            const std::vector<const char*> extensionsNotFound = checkRequirementsExtensions(_instanceInfo, requirements.optionalInstanceExtensions, extensions);

            for (const char* extensionName : extensionsNotFound) {
                LUG_LOG.warn("Can't load optional extension '{}' for module '{}'", extensionName, moduleType);
            }
        }

        if (moduleRequirementsCheck) {
            _loadedInstanceLayers.insert(_loadedInstanceLayers.end(), layers.begin(), layers.end());
            _loadedInstanceExtensions.insert(_loadedInstanceExtensions.end(), extensions.begin(), extensions.end());
        } else {
            loadedModules.erase(moduleType);
        }

        requirementsCheck = requirementsCheck && moduleRequirementsCheck;
    }

    return requirementsCheck;
}

bool Renderer::checkRequirementsDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules, bool finalization) {
    bool requirementsCheck = true;

    for (const auto moduleType : modulesToCheck) {
        const auto& requirements = modulesRequirements.at(moduleType);

        std::vector<const char*> extensions{};
        VkPhysicalDeviceFeatures features{};
        std::set<int8_t> queueFamiliesIdx{};

        bool moduleRequirementsCheck = true;

        // Check extensions
        {
            const std::vector<const char*> extensionsNotFound = checkRequirementsExtensions(physicalDeviceInfo, requirements.mandatoryDeviceExtensions, extensions);
            moduleRequirementsCheck = moduleRequirementsCheck && extensionsNotFound.size() == 0;

            if (!finalization) {
                for (const char* const extensionName : extensionsNotFound) {
                    LUG_LOG.warn("Device {}: Can't load mandatory extension '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, extensionName, moduleType);
                }
            }
        }

        {
            const std::vector<const char*> extensionsNotFound = checkRequirementsExtensions(physicalDeviceInfo, requirements.optionalDeviceExtensions, extensions);

            if (!finalization) {
                for (const char* extensionName : extensionsNotFound) {
                    LUG_LOG.warn("Device {}: Can't load optional extension '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, extensionName, moduleType);
                }
            }
        }

        // TODO: Log error
        #define LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES(featureName)                                                                            \
            {                                                                                                                                               \
                if (requirements.mandatoryFeatures.featureName == VK_TRUE) {                                                                                \
                    if (physicalDeviceInfo.features.featureName == VK_TRUE) {                                                                               \
                        features.featureName = VK_TRUE;                                                                                                     \
                    } else {                                                                                                                                \
                        if (!finalization) {                                                                                                                \
                            LUG_LOG.warn("Device {}: Can't load mandatory feature '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, #featureName, moduleType); \
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
                if (requirements.optionalFeatures.featureName == VK_TRUE) {                                                                             \
                    if (physicalDeviceInfo.features.featureName == VK_TRUE) {                                                                           \
                        features.featureName = VK_TRUE;                                                                                                 \
                    } else if (!finalization) {                                                                                                         \
                        LUG_LOG.warn("Device {}: Can't load optional feature '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, #featureName, moduleType); \
                    }                                                                                                                                   \
                }                                                                                                                                       \
            }
        LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_CHECK_VULKAN_PHYSICAL_DEVICE_OPTIONNAL_FEATURES);
        #undef LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES

        for (const auto& queueFlags : requirements.mandatoryQueueFlags) {
            int8_t idx = 0;
            if (physicalDeviceInfo.containsQueueFlags(queueFlags, idx)) {
                queueFamiliesIdx.insert(idx);
            } else {
                if (!finalization) {
                    LUG_LOG.warn("Device {}: Can't find mandatory queue type for module '{}'", physicalDeviceInfo.properties.deviceName, moduleType);
                }

                moduleRequirementsCheck = false;
            }
        }

        for (const auto& queueFlags : requirements.optionalQueueFlags) {
            int8_t idx = 0;
            if (physicalDeviceInfo.containsQueueFlags(queueFlags, idx)) {
                queueFamiliesIdx.insert(idx);
            } else if (!finalization) {
                LUG_LOG.warn("Device {}: Can't find optional queue type for module '{}'", physicalDeviceInfo.properties.deviceName, moduleType);
            }
        }

        if (finalization) {
            if (moduleRequirementsCheck) {
                _loadedDeviceExtensions.insert(_loadedDeviceExtensions.end(), extensions.begin(), extensions.end());

                #define LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES(featureName) _loadedDeviceFeatures.featureName = _loadedDeviceFeatures.featureName || features.featureName;
                LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES);
                #undef LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES

                _loadedQueueFamiliesIdx.insert(queueFamiliesIdx.begin(), queueFamiliesIdx.end());
            } else {
                loadedModules.erase(moduleType);
            }
        }

        requirementsCheck = requirementsCheck && moduleRequirementsCheck;
    }

    return requirementsCheck;
}

template <typename Info>
inline std::vector<const char*> Renderer::checkRequirementsLayers(const Info& info, const std::vector<const char*>& layers, std::vector<const char*>& layersFound) {
    std::vector<const char*> layersNotFound{};

    for (const char* layerName : layers) {
        if (info.containsLayer(layerName)) {
            layersFound.push_back(layerName);
        } else {
            layersNotFound.push_back(layerName);
        }
    }

    return layersNotFound;
}

template <typename Info>
inline std::vector<const char*> Renderer::checkRequirementsExtensions(const Info& info, const std::vector<const char*>& extensions, std::vector<const char*>& extensionsFound) {
    std::vector<const char*> extensionsNotFound{};

    for (const char* extensionName : extensions) {
        if (info.containsExtension(extensionName)) {
            extensionsFound.push_back(extensionName);
        } else {
            extensionsNotFound.push_back(extensionName);
        }
    }

    return extensionsNotFound;
}

::lug::Graphics::RenderWindow* Renderer::createWindow(RenderWindow::InitInfo& initInfo) {
    _window = RenderWindow::create(*this, initInfo);
    return _window.get();
}

::lug::Graphics::RenderWindow* Renderer::getWindow() {
    return _window.get();
}

bool Renderer::beginFrame() {
    return _window->beginFrame();
}

bool Renderer::endFrame() {
    _window->render();

    for (auto& renderView: _window->getRenderViews()) {
        if (!renderView->endFrame()) {
            return false;
        }
    }

    return _window->endFrame();
}

} // Vulkan
} // Graphics
} // lug
