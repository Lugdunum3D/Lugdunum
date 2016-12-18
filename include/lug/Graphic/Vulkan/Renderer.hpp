#pragma once

#include <algorithm>
#include <unordered_map>
#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Renderer.hpp>
#include <lug/Graphic/Vulkan/Device.hpp>
#include <lug/Graphic/Vulkan/Instance.hpp>
#include <lug/Graphic/Vulkan/Loader.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>
#include <lug/Graphic/Vulkan/Queue.hpp>

namespace lug {
namespace Graphic {

class Graphic;

namespace Vulkan {

class LUG_GRAPHIC_API Renderer final : public ::lug::Graphic::Renderer {
public:
    struct Requirements {
        const std::vector<const char*> mandatoryInstanceExtensions;
        const std::vector<const char*> mandatoryInstanceLayers;
        const std::vector<const char*> mandatoryDeviceExtensions;

        const std::vector<const char*> optionnalInstanceExtensions;
        const std::vector<const char*> optionnalInstanceLayers;
        const std::vector<const char*> optionnalDeviceExtensions;

        const VkPhysicalDeviceFeatures mandatoryFeatures;
        const VkPhysicalDeviceFeatures optionnalFeatures;

        const std::vector<VkQueueFlags> mandatoryQueueFlags;
        const std::vector<VkQueueFlags> optionnalQueueFlags;

        // TODO: PhysicalDeviceLimits / PhysicalDeviceSparseProperties
        // TODO: Memory properties
        // TODO: Better support for queues
    };

public:
    Renderer(Graphic&);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    ~Renderer();

    std::set<Module::Type> init() override final;

    bool isInstanceLayerLoaded(const char* name) const;
    bool isInstanceExtensionLoaded(const char* name) const;
    bool isDeviceExtensionLoaded(const char* name) const;

private:
    bool initInstance(std::set<Module::Type> &loadedModules);
    bool checkRequirementsInstance(const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules, bool optionnal);
    bool initDevice(std::set<Module::Type> &loadedModules);
    bool checkRequirementsDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules, bool optionnal, bool quiet);

private:
    Loader _loader; // Need to be at the beginning, we don't want to unload Vulkan functions too early
    Graphic& _graphic;

    Instance _instance{};
    Device _device{};
    std::vector<Queue> _queues{};

    InstanceInfo _instanceInfo{};
    PhysicalDeviceInfo* _physicalDeviceInfo{nullptr};
    std::vector<PhysicalDeviceInfo> _physicalDeviceInfos{};

    VkDebugReportCallbackEXT _debugReportCallback{VK_NULL_HANDLE};

    std::vector<const char*> _loadedInstanceLayers{};
    std::vector<const char*> _loadedInstanceExtensions{};
    std::vector<const char*> _loadedDeviceExtensions{};
    VkPhysicalDeviceFeatures _loadedDeviceFeatures{};
    std::set<int8_t> _loadedQueueFamiliesIdx{};

private:
    static const std::unordered_map<Module::Type, Requirements> modulesRequirements;
};

#include <lug/Graphic/Vulkan/Renderer.inl>

} // Vulkan
} // Graphic
} // lug
