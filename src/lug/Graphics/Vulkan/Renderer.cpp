#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Device.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Instance.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Graphics/Vulkan/Requirements/Core.hpp>
#include <lug/Graphics/Vulkan/Requirements/Requirements.hpp>
#include <lug/Graphics/Vulkan/Render/Window.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Transform.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

const std::unordered_map<Module::Type, Renderer::Requirements> Renderer::modulesRequirements = {
#define LUG_INIT_GRAPHICS_MODULES_REQUIREMENTS(name)                                  \
    {                                                                                 \
        Module::Type::name, ::lug::Graphics::Vulkan::Requirements::name::requirements \
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

    // Convert VkDebugReportFlagsEXT to System::Logger::Level
    System::Logger::Level level = System::Logger::Level::Off;

    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        level = System::Logger::Level::Error;
    } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        level = System::Logger::Level::Warning;
    } else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        level = System::Logger::Level::Info;
    } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        level = System::Logger::Level::Debug;
    }

    LUG_LOG.log(level, "DebugReport: {}: {}", layerPrefix, msg);

    return VK_FALSE;
}

Renderer::Renderer(Graphics& graphics) : ::lug::Graphics::Renderer(graphics) {}

Renderer::~Renderer() {
    destroy();
}

void Renderer::destroy() {
    // Destroy the window
    _window.reset();

    _device.destroy();

    // Destroy the report callback if necessary
    {
        if (isInstanceExtensionLoaded(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
            auto vkDestroyDebugReportCallbackEXT = _instance.getProcAddr<PFN_vkDestroyDebugReportCallbackEXT>("vkDestroyDebugReportCallbackEXT");

            if (vkDestroyDebugReportCallbackEXT) {
                vkDestroyDebugReportCallbackEXT(static_cast<VkInstance>(_instance), _debugReportCallback, nullptr);
            }
        }
    }

    _instance.destroy();
    _loader.unload();
}

bool Renderer::beginInit(const std::string& appName, const Core::Version& appVersion, const Renderer::InitInfo& initInfo) {
    _initInfo = initInfo;

    if (!initInstance(appName, appVersion)) {
        LUG_LOG.error("RendererVulkan: Can't init the instance");
        return false;
    }

    return true;
}

bool Renderer::finishInit() {
    // Is it a second time finishInit?
    if (static_cast<VkDevice>(_device)) {
        _device.waitIdle();

        // Destroy the render part of the window
        if (_window) {
            _window->destroyRender();
        }

        _device.destroy();
    }

    if (!initDevice()) {
        LUG_LOG.error("RendererVulkan: Can't init the device");
        return false;
    }

#if defined(LUG_DEBUG)
    LUG_LOG.info("RendererVulkan: Use device {}", _physicalDeviceInfo->properties.deviceName);
#endif

    return true;
}

bool Renderer::initInstance(const std::string& appName, const Core::Version& appVersion) {
    VkResult result{VK_SUCCESS};

    // Load vulkan core functions
    {
        if (!_loader.loadCoreFunctions()) {
            LUG_LOG.error("RendererVulkan: Can't load core vulkan functions");
            return false;
        }
    }

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
        if (!checkRequirementsInstance(_graphics.getLoadedMandatoryModules())) {
            return false;
        }

        checkRequirementsInstance(_graphics.getLoadedOptionalModules());

        // Build the instance
        API::Builder::Instance instanceBuilder;

        instanceBuilder.setApplicationInfo(appName, appVersion);
        instanceBuilder.setEngineInfo(LUG_NAME, {LUG_VERSION_MAJOR, LUG_VERSION_MINOR, LUG_VERSION_PATCH});

        instanceBuilder.setLayers(_loadedInstanceLayers);
        instanceBuilder.setExtensions(_loadedInstanceExtensions);

        if (!instanceBuilder.build(_instance, &result)) {
            LUG_LOG.error("RendererVulkan: Can't create the instance: {}", result);
            return false;
        }

    }

    // Create report callback if necessary
    {
        if (isInstanceExtensionLoaded(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
            VkDebugReportCallbackCreateInfoEXT createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            // We don't want VK_DEBUG_REPORT_INFORMATION_BIT_EXT and not VK_DEBUG_REPORT_DEBUG_BIT_EXT
            createInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
            createInfo.pfnCallback = debugReportCallback;

            auto vkCreateDebugReportCallbackEXT = _instance.getProcAddr<PFN_vkCreateDebugReportCallbackEXT>("vkCreateDebugReportCallbackEXT");

            if (vkCreateDebugReportCallbackEXT) {
                vkCreateDebugReportCallbackEXT(static_cast<VkInstance>(_instance), &createInfo, nullptr, &_debugReportCallback);
            } else {
                LUG_LOG.warn("RendererVulkan: Can't load function vkCreateDebugReportCallbackEXT");
            }
        }
    }

    // Load vulkan instance functions
    {
        if (!_loader.loadInstanceFunctions(_instance)) {
            LUG_LOG.error("RendererVulkan: Can't load instance vulkan functions");
            return false;
        }
    }

    // Load physical devices information
    {
        uint32_t physicalDevicesCount = 0;
        result = vkEnumeratePhysicalDevices(static_cast<VkInstance>(_instance), &physicalDevicesCount, nullptr);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererVulkan: Can't enumerate physical devices: {}", result);
            return false;
        }

        _physicalDeviceInfos.resize(physicalDevicesCount);

        std::vector<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
        result = vkEnumeratePhysicalDevices(static_cast<VkInstance>(_instance), &physicalDevicesCount, physicalDevices.data());

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
            {
#define LUG_LOAD_IMAGE_FORMAT_PROPERTIES(formatEnum) \
    vkGetPhysicalDeviceFormatProperties(physicalDevices[idx], formatEnum, &_physicalDeviceInfos[idx].formatProperties[formatEnum]);
                LUG_VULKAN_FORMAT(LUG_LOAD_IMAGE_FORMAT_PROPERTIES)
#undef LUG_LOAD_IMAGE_FORMAT_PROPERTIES
            }

            // TODO: Get additional informations (images properties, etc)
        }
    }

    return true;
}

bool Renderer::initDevice() {
    VkResult result{VK_SUCCESS};

    if (_preferences.device) {
        _physicalDeviceInfo = _preferences.device;

        // Set the loaded informations of the device
        {
            if (!checkRequirementsDevice(*_physicalDeviceInfo, _graphics.getLoadedMandatoryModules(), true, false)) {
                return false;
            }

            checkRequirementsDevice(*_physicalDeviceInfo, _graphics.getLoadedOptionalModules(), true, false);
        }
    } else {
        // Select device
        {
            uint8_t matchedDeviceIdx = 0;
            std::vector<uint8_t> matchedDevicesIdx{};

            for (uint8_t idx = 0; idx < _physicalDeviceInfos.size(); ++idx) {
                if (!checkRequirementsDevice(_physicalDeviceInfos[idx], _graphics.getLoadedMandatoryModules(), false, false)) {
                    continue;
                }

                checkRequirementsDevice(_physicalDeviceInfos[idx], _graphics.getLoadedOptionalModules(), false, false);

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
            checkRequirementsDevice(*_physicalDeviceInfo, _graphics.getLoadedMandatoryModules(), true, true);
            checkRequirementsDevice(*_physicalDeviceInfo, _graphics.getLoadedOptionalModules(), true, true);
        }
    }

    // Create device
    {
        // Build the device
        API::Builder::Device deviceBuilder(*_physicalDeviceInfo);

        deviceBuilder.setExtensions(_loadedDeviceExtensions);
        deviceBuilder.setFeatures(_loadedDeviceFeatures);

        if (!deviceBuilder.addQueues(VK_QUEUE_GRAPHICS_BIT, {"queue_graphics"}) ||
            !deviceBuilder.addQueues(VK_QUEUE_TRANSFER_BIT, {"queue_transfer"})) {
            return false;
        }

        if (!deviceBuilder.build(_device, &result)) {
            LUG_LOG.error("RendererVulkan: Can't create the device: {}", result);
            return false;
        }
    }

    // Load vulkan device functions
    {
        if (!_loader.loadDeviceFunctions(_device)) {
            LUG_LOG.error("RendererVulkan: Can't load device vulkan functions");
            return false;
        }
    }

    return true;
}

bool Renderer::checkRequirementsInstance(const std::set<Module::Type>& modulesToCheck) {
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
            _graphics.unsupportedModule(moduleType);
        }

        requirementsCheck = requirementsCheck && moduleRequirementsCheck;
    }

    return requirementsCheck;
}

bool Renderer::checkRequirementsDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const std::set<Module::Type>& modulesToCheck, bool finalization, bool quiet) {
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

            if (!quiet) {
                for (const char* const extensionName : extensionsNotFound) {
                    LUG_LOG.warn("Device {}: Can't load mandatory extension '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, extensionName, moduleType);
                }
            }
        }

        {
            const std::vector<const char*> extensionsNotFound = checkRequirementsExtensions(physicalDeviceInfo, requirements.optionalDeviceExtensions, extensions);

            if (!quiet) {
                for (const char* extensionName : extensionsNotFound) {
                    LUG_LOG.warn("Device {}: Can't load optional extension '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, extensionName, moduleType);
                }
            }
        }

        // TODO: Log error
#define LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES(featureName)                                                                                              \
    {                                                                                                                                                                 \
        if (requirements.mandatoryFeatures.featureName == VK_TRUE) {                                                                                                  \
            if (physicalDeviceInfo.features.featureName == VK_TRUE) {                                                                                                 \
                features.featureName = VK_TRUE;                                                                                                                       \
            } else {                                                                                                                                                  \
                if (!quiet) {                                                                                                                                         \
                    LUG_LOG.warn("Device {}: Can't load mandatory feature '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, #featureName, moduleType); \
                }                                                                                                                                                     \
                moduleRequirementsCheck = false;                                                                                                                      \
            }                                                                                                                                                         \
        }                                                                                                                                                             \
    }
        LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES);
#undef LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES

        // TODO: Log error
#define LUG_CHECK_VULKAN_PHYSICAL_DEVICE_OPTIONNAL_FEATURES(featureName)                                                                                         \
    {                                                                                                                                                            \
        if (requirements.optionalFeatures.featureName == VK_TRUE) {                                                                                              \
            if (physicalDeviceInfo.features.featureName == VK_TRUE) {                                                                                            \
                features.featureName = VK_TRUE;                                                                                                                  \
            } else if (!quiet) {                                                                                                                                 \
                LUG_LOG.warn("Device {}: Can't load optional feature '{}' for module '{}'", physicalDeviceInfo.properties.deviceName, #featureName, moduleType); \
            }                                                                                                                                                    \
        }                                                                                                                                                        \
    }
        LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_CHECK_VULKAN_PHYSICAL_DEVICE_OPTIONNAL_FEATURES);
#undef LUG_CHECK_VULKAN_PHYSICAL_DEVICE_MANDATORY_FEATURES

        if (finalization) {
            if (moduleRequirementsCheck) {
                _loadedDeviceExtensions.insert(_loadedDeviceExtensions.end(), extensions.begin(), extensions.end());

#define LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES(featureName) _loadedDeviceFeatures.featureName = _loadedDeviceFeatures.featureName || features.featureName;
                LUG_VULKAN_PHYSICAL_DEVICE_FEATURES(LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES);
#undef LUG_FILL_VULKAN_PHYSICAL_DEVICE_FEATURES
            } else {
                _graphics.unsupportedModule(moduleType);
            }
        }

        requirementsCheck = requirementsCheck && moduleRequirementsCheck;
    }

    return requirementsCheck;
}

template<typename Info>
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

template<typename Info>
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

::lug::Graphics::Render::Window* Renderer::createWindow(Render::Window::InitInfo& initInfo) {
    if (_window) {
        if (!_window->initRender()) {
            _window.reset();
        }
    } else {
        _window = Render::Window::create(*this, initInfo);
    }

    return _window.get();
}

::lug::Graphics::Render::Window* Renderer::getWindow() {
    return _window.get();
}

bool Renderer::beginFrame(const lug::System::Time &elapsedTime) {
    return _window->beginFrame(elapsedTime);
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
