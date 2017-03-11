#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Window.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/API/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Semaphore.hpp>
#include <lug/Graphics/Vulkan/API/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

namespace Render {

class LUG_GRAPHICS_API Window final : public ::lug::Graphics::Render::Window {
private:
    struct AcquireImageData {
        API::Semaphore completeSemaphore;
        int imageIdx = -1;
    };

    struct FrameData {
        API::Semaphore allDrawsFinishedSemaphore{};
        std::vector<API::Semaphore> imageReadySemaphores{};
        std::vector<API::CommandBuffer> cmdBuffers;
    };

public:
    Window() = delete;

    Window(Renderer& renderer);

    Window(const Window&) = delete;
    Window(Window&&) = delete;

    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    ~Window();

    bool pollEvent(lug::Window::Event&) override;

    bool beginFrame() override final;
    bool endFrame() override final;

    const API::Swapchain& getSwapchain() const;

    ::lug::Graphics::Render::View* createView(::lug::Graphics::Render::View::InitInfo& initInfo) override final;

    bool render() override final;

    uint16_t getWidth() const override final;
    uint16_t getHeight() const override final;

    static std::unique_ptr<Window> create(Renderer& renderer, Window::InitInfo& initInfo);

    bool initRender();
    void destroyRender();

private:
    bool init(Window::InitInfo& initInfo);
    bool initDescriptorPool();
    bool initSurface();
    bool initSwapchainCapabilities();
    bool initPresentQueue();
    bool initSwapchain();
    bool initFramesData();
    bool buildCommandBuffers();

private:
    InitInfo _initInfo;

    Renderer& _renderer;
    VkSurfaceKHR _surface{VK_NULL_HANDLE};
    API::Swapchain _swapchain{};

    std::unique_ptr<API::DescriptorPool> _descriptorPool{nullptr};

    API::Queue* _presentQueue{nullptr};
    uint32_t _currentImageIndex{0};

    std::vector<FrameData> _framesData;

    std::vector<AcquireImageData> _acquireImageDatas;
};

#include <lug/Graphics/Vulkan/Render/Window.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
