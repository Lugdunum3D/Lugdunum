#pragma once

#include <memory>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Instance.hpp>
#include <lug/Graphics/Vulkan/Loader.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Pipeline.hpp>

namespace lug {
namespace Graphics {

class Graphics;

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
    Renderer(Graphics&);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    ~Renderer();

    std::set<Module::Type> init() override final;
    bool lateInit();

    bool isInstanceLayerLoaded(const char* name) const;
    bool isInstanceExtensionLoaded(const char* name) const;
    bool isDeviceExtensionLoaded(const char* name) const;

    std::unique_ptr<::lug::Graphics::RenderWindow> createWindow(uint16_t width, uint16_t height, const std::string& title, lug::Window::Style style) override final;

    const Instance& getInstance() const;
    const Device& getDevice() const;
    std::vector<Queue>& getQueues();
    const std::vector<Queue>& getQueues() const;
    Queue* getQueue(VkQueueFlags flags, bool supportPresentation);
    const Queue* getQueue(VkQueueFlags flags, bool supportPresentation) const;

    std::vector<CommandBuffer>& getCommandBuffers();

    bool isSameQueue(VkQueueFlags flagsA, bool supportPresentationA, VkQueueFlags flagsB, bool supportPresentationB) const;

    InstanceInfo& getInstanceInfo();
    const InstanceInfo& getInstanceInfo() const;

    PhysicalDeviceInfo* getPhysicalDeviceInfo();
    const PhysicalDeviceInfo* getPhysicalDeviceInfo() const;

    void destroy();

    void setGraphicsPipeline(std::unique_ptr<Pipeline> graphicsPipeline);
    Pipeline* getGraphicsPipeline() const;

    bool beginFrame(const Swapchain& swapChain, uint32_t currentImageIndex);
    bool endFrame();

private:
    bool initInstance(std::set<Module::Type> &loadedModules);
    bool initDevice(std::set<Module::Type> &loadedModules);

    bool checkRequirementsInstance(const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules);
    bool checkRequirementsDevice(const PhysicalDeviceInfo& physicalDeviceInfo, const std::set<Module::Type> &modulesToCheck, std::set<Module::Type> &loadedModules, bool finalization);

    template <typename Info>
    std::vector<const char*> checkRequirementsLayers(const Info& info, const std::vector<const char*>& layers, std::vector<const char*>& layersFound);

    template <typename Info>
    std::vector<const char*> checkRequirementsExtensions(const Info& info, const std::vector<const char*>& extensions, std::vector<const char*>& extensionsFound);

private:
    Loader _loader; // Need to be at the beginning, we don't want to unload Vulkan functions too early
    Graphics& _graphic;

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

    std::vector<CommandBuffer> _cmdBuffers;
    std::unique_ptr<Pipeline> _graphicsPipeline{nullptr};


    std::unique_ptr<DeviceMemory> _vertexDeviceMemory{nullptr};
    std::unique_ptr<Buffer> _vertexBuffer{nullptr};
    std::unique_ptr<DeviceMemory> _indexDeviceMemory{nullptr};
    std::unique_ptr<Buffer> _indexBuffer{nullptr};

private:
    static const std::unordered_map<Module::Type, Requirements> modulesRequirements;
};

#include <lug/Graphics/Vulkan/Renderer.inl>

} // Vulkan
} // Graphics
} // lug
