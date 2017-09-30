#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Semaphore.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Image.hpp>
#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/QueueFamily.hpp>
#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Sampler.hpp>
#include <lug/Graphics/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/API/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Queue.hpp>
#include <lug/Graphics/Render/Window.hpp>
#include <lug/System/Time.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace Render {
    namespace DescriptorSetPool {
        class GuiTexture;
        class DescriptorSet;
    };
    class Window;
} // Render

class Renderer;

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

        std::vector<const Render::DescriptorSetPool::DescriptorSet*> texturesDescriptorSets;

        Vulkan::API::DeviceMemory vertexMemory;
        Vulkan::API::DeviceMemory indexMemory;

        Vulkan::API::Buffer vertexBuffer;
        Vulkan::API::Buffer indexBuffer;

        void* vertexMemoryPtr{nullptr};
        void* indexMemoryPtr{nullptr};
    };

public:
    Gui(lug::Graphics::Vulkan::Renderer& renderer, lug::Graphics::Vulkan::Render::Window &window);

    Gui(const Gui&) = delete;
    Gui(Gui&& gui) = delete;

    Gui& operator=(const Gui&) = delete;
    Gui& operator=(Gui&& fence) = delete;

    ~Gui();

    void destroy();

    bool init(const std::vector<API::ImageView>& imageViews);
    void initKeyMapping();
    bool initFontsTexture();
    bool initPipeline();
    bool initFramebuffers(const std::vector<API::ImageView>&);

    void beginFrame(const lug::System::Time& elapsedTime);
    bool endFrame(const std::vector<VkSemaphore>& waitSemaphores, uint32_t currentImageIndex);

    void processEvent(const lug::Window::Event event);

    const Vulkan::API::Semaphore& getSemaphore(uint32_t currentImageIndex) const;

private:
    bool updateBuffers(uint32_t currentImageIndex);
    bool initFrameData();

private:
    lug::Graphics::Vulkan::Renderer& _renderer;
    lug::Graphics::Vulkan::Render::Window& _window;

    const API::Queue* _graphicQueue{nullptr};
    const API::Queue* _transferQueue{nullptr};

    std::unique_ptr<Render::DescriptorSetPool::GuiTexture> _texturesDescriptorSetPool;

    API::CommandPool _graphicQueueCommandPool{};
    API::CommandPool _transferQueueCommandPool{};

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _fontTexture;

    API::DescriptorPool _descriptorPool;
    API::DescriptorSet _descriptorSet;

    API::PipelineLayout _pipelineLayout;
    API::GraphicsPipeline _pipeline;

    std::vector<FrameData> _framesData;
};

} // Vulkan
} // Graphics
} // lug
