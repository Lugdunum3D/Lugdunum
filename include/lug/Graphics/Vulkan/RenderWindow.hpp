#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderWindow.hpp>
#include <lug/Graphics/Vulkan/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/Fence.hpp>
#include <lug/Graphics/Vulkan/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

class LUG_GRAPHICS_API RenderWindow final : public ::lug::Graphics::RenderWindow {
private:
    struct FrameData {
        Semaphore allDrawsFinishedSemaphore{};
        std::vector<Semaphore> imageReadySemaphores{};
        std::vector<CommandBuffer> cmdBuffers;
    };

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

    lug::Graphics::RenderView* createView(lug::Graphics::RenderView::InitInfo& initInfo) override final;

    bool render() override final;

    uint16_t getWidth() const override final;
    uint16_t getHeight() const override final;

    static std::unique_ptr<RenderWindow> create(Renderer& renderer, RenderWindow::InitInfo& initInfo);

private:
    bool init(RenderWindow::InitInfo& initInfo);
    bool initSurface();
    bool initSwapchainCapabilities();
    bool initPresentQueue();
    bool initSwapchain();
    bool initFramesData(RenderWindow::InitInfo& initInfo);
    bool buildCommandBuffers();

    void destroy();

private:
    Renderer& _renderer;
    VkSurfaceKHR _surface{VK_NULL_HANDLE};
    Swapchain _swapchain{};

    std::unique_ptr<::lug::Graphics::Vulkan::DescriptorPool> _descriptorPool{nullptr};

    Queue* _presentQueue{nullptr};
    uint32_t _currentImageIndex{0};

    std::vector<FrameData> _framesData;

    std::vector<Semaphore> _acquireImageCompleteSemaphores;
    uint32_t _imageCompleteSemaphoreIdx = 0;
};

#include <lug/Graphics/Vulkan/RenderWindow.inl>

} // Vulkan
} // Graphics
} // lug
