#pragma once

#include <memory>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Instance.hpp>
#include <lug/Graphics/Vulkan/API/Loader.hpp>
#include <lug/Graphics/Vulkan/API/Pipeline.hpp>
#include <lug/Graphics/Vulkan/API/Queue.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>
#include <lug/Graphics/Vulkan/Render/Window.hpp>
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

    struct Preferences {
        PhysicalDeviceInfo* device;

        struct Swapchain {
            std::vector<VkPresentModeKHR> presentModes;                 // By order of preference
            std::vector<VkFormat> formats;                              // By order of preference
            std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphas;   // By order of preference
        } swapchain;
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

    ::lug::Graphics::Render::Window* createWindow(Render::Window::InitInfo& initInfo) override final;
    ::lug::Graphics::Render::Window* getWindow() override final;

    const API::Instance& getInstance() const;
    const API::Device& getDevice() const;
    std::vector<API::Queue>& getQueues();
    const std::vector<API::Queue>& getQueues() const;
    API::Queue* getQueue(VkQueueFlags flags, bool supportPresentation);
    const API::Queue* getQueue(VkQueueFlags flags, bool supportPresentation) const;

    bool isSameQueue(VkQueueFlags flagsA, bool supportPresentationA, VkQueueFlags flagsB, bool supportPresentationB) const;

    InstanceInfo& getInstanceInfo();
    const InstanceInfo& getInstanceInfo() const;

    PhysicalDeviceInfo* getPhysicalDeviceInfo();
    const PhysicalDeviceInfo* getPhysicalDeviceInfo() const;

    std::vector<PhysicalDeviceInfo>& getPhysicalDeviceInfos();
    const std::vector<PhysicalDeviceInfo>& getPhysicalDeviceInfos() const;

    Preferences& getPreferences();
    const Preferences& getPreferences() const;

    void destroy();

    bool beginFrame() override final;
    bool endFrame() override final;

private:
    bool initInstance(const char* appName, uint32_t appVersion);
    bool initDevice();

    bool checkRequirementsInstance(const std::set<Module::Type> &modulesToCheck);
    bool checkRequirementsDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const std::set<Module::Type> &modulesToCheck, bool finalization, bool quiet);

    template <typename Info>
    std::vector<const char*> checkRequirementsLayers(const Info& info, const std::vector<const char*>& layers, std::vector<const char*>& layersFound);

    template <typename Info>
    std::vector<const char*> checkRequirementsExtensions(const Info& info, const std::vector<const char*>& extensions, std::vector<const char*>& extensionsFound);

private:
    API::Loader _loader;

    API::Instance _instance{};
    API::Device _device{};
    std::vector<API::Queue> _queues{};

    InstanceInfo _instanceInfo{};
    PhysicalDeviceInfo* _physicalDeviceInfo{nullptr};
    std::vector<PhysicalDeviceInfo> _physicalDeviceInfos{};

    VkDebugReportCallbackEXT _debugReportCallback{VK_NULL_HANDLE};

    std::unique_ptr<Render::Window> _window;

    std::vector<const char*> _loadedInstanceLayers{};
    std::vector<const char*> _loadedInstanceExtensions{};
    std::vector<const char*> _loadedDeviceExtensions{};
    VkPhysicalDeviceFeatures _loadedDeviceFeatures{};
    std::set<int8_t> _loadedQueueFamiliesIdx{};
    std::vector<Render::Mesh*> _attachedMeshes{};

    Preferences _preferences{
        nullptr,                                    // device

        {                                           // swapchain
            {                                       // presentModes
                VK_PRESENT_MODE_MAILBOX_KHR,
                VK_PRESENT_MODE_FIFO_KHR
            },
            {                                       // formats
                VK_FORMAT_B8G8R8A8_UNORM,
                VK_FORMAT_R8G8B8A8_UNORM
            },
            {                                       // compositeAlphas
                VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
            }
        }
    };

    std::unordered_map<Render::Pipeline::Id, Resource::WeakPtr<Render::Pipeline>> _pipelines;

private:
    static const std::unordered_map<Module::Type, Requirements> modulesRequirements;
};

#include <lug/Graphics/Vulkan/Renderer.inl>

} // Vulkan
} // Graphics
} // lug
