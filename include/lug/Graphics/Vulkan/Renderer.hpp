#pragma once

#include <memory>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Instance.hpp>
#include <lug/Graphics/Vulkan/Loader.hpp>
#include <lug/Graphics/Vulkan/Mesh.hpp>
#include <lug/Graphics/Vulkan/Pipeline.hpp>
#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/Graphics/Vulkan/RenderWindow.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Renderer final : public ::lug::Graphics::Renderer {
public:
    struct Requirements {
        const std::vector<const char*> mandatoryInstanceExtensions;
        const std::vector<const char*> mandatoryInstanceLayers;
        const std::vector<const char*> mandatoryDeviceExtensions;

        const std::vector<const char*> optionalInstanceExtensions;
        const std::vector<const char*> optionalInstanceLayers;
        const std::vector<const char*> optionalDeviceExtensions;

        const VkPhysicalDeviceFeatures mandatoryFeatures;
        const VkPhysicalDeviceFeatures optionalFeatures;

        const std::vector<VkQueueFlags> mandatoryQueueFlags;
        const std::vector<VkQueueFlags> optionalQueueFlags;

        // TODO: PhysicalDeviceLimits / PhysicalDeviceSparseProperties
        // TODO: Memory properties
        // TODO: Better support for queues
    };

public:
    explicit Renderer(Graphics& graphics);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    ~Renderer();

    bool beginInit(const char* appName, uint32_t appVersion, const Renderer::InitInfo& initInfo) override final;
    bool finishInit() override final;

    bool isInstanceLayerLoaded(const char* name) const;
    bool isInstanceExtensionLoaded(const char* name) const;
    bool isDeviceExtensionLoaded(const char* name) const;

    ::lug::Graphics::RenderWindow* createWindow(RenderWindow::InitInfo& initInfo) override final;
    ::lug::Graphics::RenderWindow* getWindow() override final;

    const Instance& getInstance() const;
    const Device& getDevice() const;
    std::vector<Queue>& getQueues();
    const std::vector<Queue>& getQueues() const;
    Queue* getQueue(VkQueueFlags flags, bool supportPresentation);
    const Queue* getQueue(VkQueueFlags flags, bool supportPresentation) const;

    bool isSameQueue(VkQueueFlags flagsA, bool supportPresentationA, VkQueueFlags flagsB, bool supportPresentationB) const;

    InstanceInfo& getInstanceInfo();
    const InstanceInfo& getInstanceInfo() const;

    PhysicalDeviceInfo* getPhysicalDeviceInfo();
    const PhysicalDeviceInfo* getPhysicalDeviceInfo() const;

    void destroy();

    bool beginFrame() override final;
    bool endFrame() override final;

private:
    bool initInstance(const char* appName, uint32_t appVersion);
    bool initDevice();

    bool checkRequirementsInstance(const std::set<Module::Type> &modulesToCheck);
    bool checkRequirementsDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const std::set<Module::Type> &modulesToCheck, bool finalization);

    template <typename Info>
    std::vector<const char*> checkRequirementsLayers(const Info& info, const std::vector<const char*>& layers, std::vector<const char*>& layersFound);

    template <typename Info>
    std::vector<const char*> checkRequirementsExtensions(const Info& info, const std::vector<const char*>& extensions, std::vector<const char*>& extensionsFound);

private:
    Loader _loader;

    Instance _instance{};
    Device _device{};
    std::vector<Queue> _queues{};

    InstanceInfo _instanceInfo{};
    PhysicalDeviceInfo* _physicalDeviceInfo{nullptr};
    std::vector<PhysicalDeviceInfo> _physicalDeviceInfos{};

    VkDebugReportCallbackEXT _debugReportCallback{VK_NULL_HANDLE};

    std::unique_ptr<::lug::Graphics::Vulkan::RenderWindow> _window;

    std::vector<const char*> _loadedInstanceLayers{};
    std::vector<const char*> _loadedInstanceExtensions{};
    std::vector<const char*> _loadedDeviceExtensions{};
    VkPhysicalDeviceFeatures _loadedDeviceFeatures{};
    std::set<int8_t> _loadedQueueFamiliesIdx{};
    std::vector<Mesh*> _attachedMeshes{};

private:
    static const std::unordered_map<Module::Type, Requirements> modulesRequirements;
};

#include <lug/Graphics/Vulkan/Renderer.inl>

} // Vulkan
} // Graphics
} // lug
