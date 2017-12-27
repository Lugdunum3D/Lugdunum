#pragma once

#include <memory>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Window.hpp>
#include <lug/Graphics/Vulkan/API/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Semaphore.hpp>
#include <lug/Graphics/Vulkan/API/Surface.hpp>
#include <lug/Graphics/Vulkan/API/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/Graphics/Vulkan/Gui.hpp>
#include <lug/System/ThreadPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class QueueFamily;
} // API

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

    Window(lug::Graphics::Vulkan::Renderer& renderer);

    Window(const Window&) = delete;
    Window(Window&&) = delete;

    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    ~Window();

    bool pollEvent(lug::Window::Event&) override;

    bool beginFrame(const lug::System::Time& elapsedTime) override final;
    bool endFrame() override final;

    const API::Swapchain& getSwapchain() const;

    ::lug::Graphics::Render::View* createView(::lug::Graphics::Render::View::InitInfo& initInfo) override final;

    bool render() override final;

    uint16_t getWidth() const override final;
    uint16_t getHeight() const override final;

    static std::unique_ptr<Window> create(lug::Graphics::Vulkan::Renderer& renderer, Window::InitInfo& initInfo);

    bool initRender();
    void destroyRender();

    bool initGui();

private:
    bool init(Window::InitInfo& initInfo);
    bool initSurface();
    bool initSwapchainCapabilities();
    bool initPresentQueue();
    bool initSwapchain();
    bool initFramesData();
    bool initOffscreenData();

    bool buildBeginCommandBuffer();
    bool buildEndCommandBuffer();
    bool buildCommandBuffers();

private:
    InitInfo _initInfo;

    lug::Graphics::Vulkan::Renderer& _renderer;
    API::Surface _surface{};
    API::Swapchain _swapchain{};

    std::unique_ptr<lug::System::ThreadPool> _threadPool{nullptr};

    const API::Queue* _presentQueue{nullptr};
    const API::QueueFamily* _presentQueueFamily{nullptr};
    uint32_t _currentImageIndex{0};

    std::vector<FrameData> _framesData;

    std::vector<AcquireImageData> _acquireImageDatas;

    API::CommandPool _commandPool{};

    lug::Graphics::Vulkan::Gui  _guiInstance;
    bool _isGuiInitialized;


    std::vector<API::Image> _offscreenImages;
    std::vector<API::ImageView> _offscreenImagesViews;
    API::DeviceMemory _offscreenImagesMemory;

};

#include <lug/Graphics/Vulkan/Render/Window.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
