#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderWindow.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Fence.hpp>
#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API RenderWindow final : public ::lug::Graphics::RenderWindow {
public:
    RenderWindow() = delete;

    RenderWindow(Renderer& renderer);

    RenderWindow(const RenderWindow&) = delete;
    RenderWindow(RenderWindow&&) = delete;

    RenderWindow& operator=(const RenderWindow&) = delete;
    RenderWindow& operator=(RenderWindow&&) = delete;

    ~RenderWindow();

    bool beginFrame() override final;
    bool endFrame() override final;

    void clearScreen(float color[4]) override final;

    static std::unique_ptr<RenderWindow> create(Renderer& renderer, uint16_t width, uint16_t height, const std::string& title, lug::Window::Style style);

private:
    bool init();
    bool initSurface();
    bool initSwapchain();
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
};

} // Vulkan
} // Graphics
} // lug
