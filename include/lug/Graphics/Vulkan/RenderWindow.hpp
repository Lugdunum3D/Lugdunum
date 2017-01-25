#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderWindow.hpp>
#include <lug/Graphics/Vulkan/Fence.hpp>
#include <lug/Graphics/Vulkan/RenderView.hpp>
#include <lug/Graphics/Vulkan/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

class LUG_GRAPHICS_API RenderWindow final : public ::lug::Graphics::RenderWindow {
public:
    RenderWindow() = delete;

    RenderWindow(Renderer& renderer);

    RenderWindow(const RenderWindow&) = delete;
    RenderWindow(RenderWindow&&) = delete;

    RenderWindow& operator=(const RenderWindow&) = delete;
    RenderWindow& operator=(RenderWindow&&) = delete;

    ~RenderWindow();

    bool pollEvent(lug::Window::Event&) override;

    bool beginFrame() override final;
    bool endFrame() override final;

    const Swapchain& getSwapchain() const;
    const Framebuffer& getCurrentFramebuffer() const;

    RenderView* createView(RenderView::InitInfo& initInfo) override final;

    void render() override final;

    uint16_t getWidth() const override final;
    uint16_t getHeight() const override final;

    static std::unique_ptr<RenderWindow> create(Renderer& renderer, RenderWindow::InitInfo& initInfo);

private:
    bool init(RenderWindow::InitInfo& initInfo);
    bool initSurface();
    bool initSwapchainCapabilities();
    bool initPresentQueue();
    bool initSwapchain();
    bool initPipeline();
    void destroy();

private:
    Renderer& _renderer;
    VkSurfaceKHR _surface{VK_NULL_HANDLE};
    Swapchain _swapchain{};
    Semaphore _acquireImageCompleteSemaphore{};
    Semaphore _submitCompleteSemaphore{};
    Fence _fence;

    const Queue* _presentQueue{nullptr};
    uint32_t _currentImageIndex{0};

    std::vector<std::unique_ptr<RenderView>> _renderViews;
};

#include <lug/Graphics/Vulkan/RenderWindow.inl>

} // Vulkan
} // Graphics
} // lug
