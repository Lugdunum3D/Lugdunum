#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Sampler.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Time.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Gui {
private:
    // UI params are set via push constants
    struct PushConstBlock {
        lug::Math::Vec2f scale;
        lug::Math::Vec2f translate;
    };

    struct FrameData {
        API::Framebuffer framebuffer;
        Vulkan::API::Semaphore semaphore;
        Vulkan::API::Fence fence;
        Vulkan::API::CommandBuffer commandBuffer;

        Vulkan::API::DeviceMemory vertexMemory;
        Vulkan::API::DeviceMemory indexMemory;

        Vulkan::API::Buffer vertexBuffer;
        Vulkan::API::Buffer indexBuffer;

        void* vertexMemoryPtr{nullptr};
        void* indexMemoryPtr{nullptr};
    };

public:
    Gui() = delete;

    Gui(Renderer& renderer, Render::Window &window);

    Gui(const Gui&) = delete;

    Gui& operator=(const Gui&) = delete;
    Gui& operator=(Gui&&) = delete;

    ~Gui();

    bool init(const std::vector<API::ImageView>& imageViews);
    void initKeyMapping();
    bool initFontsTexture();
    bool initPipeline();
    bool initFramebuffers(const std::vector<API::ImageView>&);

    bool beginFrame(const lug::System::Time& elapsedTime);
    bool endFrame(const std::vector<VkSemaphore>& waitSemaphores, uint32_t currentImageIndex);

    void processEvents(lug::Window::Event event);

    const Vulkan::API::Semaphore& getGuiSemaphore(uint32_t currentImageIndex) const;

private:
    void updateBuffers(uint32_t currentImageIndex);

private:
    Renderer& _renderer;
    Render::Window& _window;

    const API::Queue* _presentQueue{nullptr};
    const API::QueueFamily* _presentQueueFamily{ nullptr};
    const API::Queue* _transferQueue{nullptr};

    API::CommandPool _commandPool{};

    API::Image _fontImage;
    API::ImageView _fontImageView;
    API::DeviceMemory _fontDeviceMemory;
    API::Sampler _fontSampler;


    API::DescriptorPool _descriptorPool;
    API::DescriptorSet _descriptorSet;

    API::PipelineLayout _pipelineLayout;
    API::GraphicsPipeline _pipeline;

    std::vector<FrameData> _framesData;
};

} // Vulkan
} // Graphics
} // lug
