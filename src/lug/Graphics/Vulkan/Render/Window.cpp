#include <cstring>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/SkyBox.hpp>
#include <lug/Graphics/Vulkan/Render/View.hpp>
#include <lug/Graphics/Vulkan/Render/Window.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Image.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Semaphore.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Surface.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Swapchain.hpp>
#include <lug/Graphics/Vulkan/API/Instance.hpp>
#include <lug/System/Logger/Logger.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
    #include <lug/Window/Win32/WindowImplWin32.hpp>
#elif defined(LUG_SYSTEM_LINUX)
    #include <lug/Window/Unix/WindowImplX11.hpp>
#elif defined(LUG_SYSTEM_ANDROID)
    #include <lug/Window/Android/WindowImplAndroid.hpp>
#endif

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Window::Window(lug::Graphics::Vulkan::Renderer& renderer) : _renderer(renderer), _guiInstance(_renderer, *this), _isGuiInitialized(false), _bloomPass(_renderer, *this) {}

Window::~Window() {
    destroyRender();
}

bool Window::pollEvent(lug::Window::Event& event) {
    if (lug::Window::Window::pollEvent(event)) {
        if (event.type == lug::Window::Event::Type::Resize) {
            _swapchain.setOutOfDate(true);

            // Update the RenderView
            for (auto& renderView : _renderViews) {
                renderView->update();
            }
        }

        if (_isGuiInitialized == true) {
            while (_guiInstance.processEvent(event)) {
                if (!lug::Window::Window::pollEvent(event)) {
                    return false;
                }
            }
        }

        return true;
    }

    return false;
}

bool Window::beginFrame(const lug::System::Time &elapsedTime) {
    AcquireImageData* acquireImageData = nullptr;
    // Retrieve free AcquireImageData
    {
        for (auto& acquireImageData_ : _acquireImageDatas) {
            if (acquireImageData_.imageIdx == -1) {
                acquireImageData = &acquireImageData_;
                break;
            }
        }
    }

    if (_isGuiInitialized == true) {
        _guiInstance.beginFrame(elapsedTime);
    }

    while (_swapchain.isOutOfDate() || !_swapchain.getNextImage(&_currentImageIndex, static_cast<VkSemaphore>(acquireImageData->completeSemaphore))) {
        if (_swapchain.isOutOfDate()) {
            if (!initSwapchainCapabilities() || !initSwapchain() || !initOffscreenData() || !buildCommandBuffers()) {
                return false;
            }

            if (_isGuiInitialized == true) {
                if (!_guiInstance.initFramebuffers(_sceneOffscreenImagesViews)) {
                    LUG_LOG.error("Window::beginFrame: Failed to initialise Gui framebuffers");
                    return false;
                }
            }

            for (auto& renderView: _renderViews) {
                View* renderView_ = static_cast<View*>(renderView.get());

                if (!renderView_->getRenderTechnique()->setSwapchainImageViews(_offscreenImagesViews)) {
                    return false;
                }

                renderView_->setDirty();
            }
        } else {
            return false;
        }
    }

    // Set previous AcquireImageData free
    {
        for (auto& acquireImageData_ : _acquireImageDatas) {
            if (acquireImageData_.imageIdx == (int)_currentImageIndex) {
                acquireImageData_.imageIdx = -1;
            }
        }
    }

    acquireImageData->imageIdx = (int)_currentImageIndex;

    FrameData& frameData = _framesData[_currentImageIndex];
    API::CommandBuffer& cmdBuffer = frameData.cmdBuffers[0];

    std::vector<VkSemaphore> imageReadyVkSemaphores(frameData.imageReadySemaphores.size());

    for (unsigned i = 0; i < frameData.imageReadySemaphores.size(); ++i) {
        imageReadyVkSemaphores[i] = static_cast<VkSemaphore>(frameData.imageReadySemaphores[i]);
    }

    return _presentQueue->submit(
        cmdBuffer,
        imageReadyVkSemaphores,
        {static_cast<VkSemaphore>(acquireImageData->completeSemaphore)},
        {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT}
    );
}

bool Window::endFrame() {
    bool uiResult = false;
    bool presentQueueResult = false;

    FrameData& frameData = _framesData[_currentImageIndex];

    API::CommandBuffer& cmdBuffer = frameData.cmdBuffers[1];
    std::vector<VkSemaphore> waitSemaphores(_renderViews.size());

    uint32_t i = 0;
    for (auto& renderView: _renderViews) {
        View* renderView_ = static_cast<View*>(renderView.get());

        // Render views with no camera don't signal the semaphore as they don't draw
        if (renderView_->getCamera()) {
            waitSemaphores[i] = static_cast<VkSemaphore>(renderView_->getDrawCompleteSemaphore(_currentImageIndex));
        } else {
            waitSemaphores[i] = static_cast<VkSemaphore>(frameData.imageReadySemaphores[i]);
        }

        ++i;
    }

    if (!_bloomPass.endFrame(waitSemaphores, _currentImageIndex)) {
        return false;
    }

    waitSemaphores = {
        static_cast<VkSemaphore>(_bloomPass.getSemaphore(_currentImageIndex))
    };

    if (_isGuiInitialized) {
        uiResult = _guiInstance.endFrame(waitSemaphores, _currentImageIndex);
        presentQueueResult = _presentQueue->submit(cmdBuffer,
                                                   { static_cast<VkSemaphore>(frameData.allDrawsFinishedSemaphore) },
                                                   { static_cast<VkSemaphore>(_guiInstance.getSemaphore(_currentImageIndex)) },
                                                   { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT });
    } else {
        uiResult = true;
        std::vector<VkPipelineStageFlags> waitDstStageMasks(waitSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);

        presentQueueResult = _presentQueue->submit(cmdBuffer,
                                                   { static_cast<VkSemaphore>(frameData.allDrawsFinishedSemaphore) },
                                                   waitSemaphores,
                                                   waitDstStageMasks);
    }

    return uiResult
        && presentQueueResult
        && _swapchain.present(_presentQueue, _currentImageIndex, static_cast<VkSemaphore>(frameData.allDrawsFinishedSemaphore));
}

lug::Graphics::Render::View* Window::createView(lug::Graphics::Render::View::InitInfo& initInfo) {
    std::unique_ptr<View> renderView = std::make_unique<View>(_renderer, this);

    if (!renderView->init(initInfo, _presentQueue, _sceneOffscreenImagesViews, _glowOffscreenImagesViews)) {
        return nullptr;
    }

    _renderViews.push_back(std::move(renderView));

    return _renderViews.back().get();
}

bool Window::render() {
    FrameData& frameData = _framesData[_currentImageIndex];
    uint32_t i = 0;
    // Contains all async results of Render::View::render
    std::vector<std::future<bool>> results(_renderViews.size());

    // Run renderView->render for all render views asynchronously
    for (auto& renderView: _renderViews) {
        auto view = static_cast<View*>(renderView.get());
        // Bind this contex + params with render function
        // Use std::ref for semaphore or it will try to copy it
        auto renderFunc = std::bind(&lug::Graphics::Vulkan::Render::View::render, view, std::ref(frameData.imageReadySemaphores[i++]), _currentImageIndex);
        // Send function to workers and save async result
        results[i - 1] = _threadPool->enqueue(renderFunc);
    }

    // Wait for jobs finish and check result
    bool success = true;
    for (auto&& result: results) {
        result.wait();
        success &= result.get();
    }

    return success;
}

std::unique_ptr<Window>

Window::create(lug::Graphics::Vulkan::Renderer& renderer, Window::InitInfo& initInfo) {
    std::unique_ptr<Window> window(new Window(renderer));

    if (!window->init(initInfo)) {
        return nullptr;
    }

    return window;
}

/**
 * Init surface
 * @return Success
 */
bool Window::initSurface() {
    VkResult result{VK_SUCCESS};
    API::Builder::Surface surfaceBuilder(_renderer.getInstance());

#if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
    surfaceBuilder.setWindowInformations(_impl->getHinstance(), _impl->getHandle());
#elif defined(LUG_SYSTEM_LINUX) // Linux surface
    surfaceBuilder.setWindowInformations(_impl->getDisplay(), _impl->getWindow());
#elif defined(LUG_SYSTEM_ANDROID) // Android Surface
    surfaceBuilder.setWindowInformations(_impl->getWindow());
#endif

    if (!surfaceBuilder.build(_surface, &result)) {
        LUG_LOG.error("RendererWindow: Can't initialize surface: {}", result);
        return false;
    }

    return true;
}

bool Window::initSwapchainCapabilities() {
    VkResult result{VK_SUCCESS};
    PhysicalDeviceInfo* info = _renderer.getPhysicalDeviceInfo();

    LUG_ASSERT(info != nullptr, "PhysicalDeviceInfo cannot be null");

    // Get swapchain capabilities
    {
        result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info->handle, static_cast<VkSurfaceKHR>(_surface), &info->swapchain.capabilities);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't get surface capabilities: {}", result);
            return false;
        }

        uint32_t formatsCount = 0;
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(info->handle, static_cast<VkSurfaceKHR>(_surface), &formatsCount, nullptr);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve formats count: {}", result);
            return false;
        }

        info->swapchain.formats.resize(formatsCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(info->handle, static_cast<VkSurfaceKHR>(_surface), &formatsCount, info->swapchain.formats.data());

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve formats: {}", result);
            return false;
        }

        uint32_t presentModesCount = 0;
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(info->handle, static_cast<VkSurfaceKHR>(_surface), &presentModesCount, nullptr);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve present modes count: {}", result);
            return false;
        }

        info->swapchain.presentModes.resize(presentModesCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(info->handle, static_cast<VkSurfaceKHR>(_surface), &presentModesCount, info->swapchain.presentModes.data());

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow: Can't retrieve present modes: {}", result);
            return false;
        }
    }

    return true;
}

bool Window::initGui() {
    if (!_guiInstance.init(_swapchain.getImagesViews())) {
        LUG_LOG.error("RendererWindow: Failed to initialise Gui");
        return false;
    }

    _isGuiInitialized = true;
    return _isGuiInitialized;
}

bool Window::initPresentQueue() {
    PhysicalDeviceInfo* info = _renderer.getPhysicalDeviceInfo();

    LUG_ASSERT(info != nullptr, "PhysicalDeviceInfo cannot be null");

    // Get present queue families
    {
        VkResult result{VK_SUCCESS};
        for (auto& queueFamily : _renderer.getDevice().getQueueFamilies()) {
            VkBool32 supported = 0;
            result = vkGetPhysicalDeviceSurfaceSupportKHR(info->handle, queueFamily.getIdx(), static_cast<VkSurfaceKHR>(_surface), &supported);

            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererWindow: Can't check if queue supports presentation: {}", result);
                return false;
            }

            queueFamily.supportsPresentation(!!supported);
        }
    }

    // Get present queue family and retrieve the first queue
    {
        _presentQueueFamily = _renderer.getDevice().getQueueFamily(0, true);
        if (!_presentQueueFamily) {
            LUG_LOG.error("Window::initPresentQueue: Can't find presentation queue family");
            return false;
        }
        _presentQueue = &_presentQueueFamily->getQueues().front();
        if (!_presentQueue) {
            LUG_LOG.error("Window::initPresentQueue: Can't find presentation queue");
            return false;
        }
    }

    // Create command pool of present queue
    {
        VkResult result{VK_SUCCESS};
        API::Builder::CommandPool commandPoolBuilder(_renderer.getDevice(), *_presentQueueFamily);
        if (!commandPoolBuilder.build(_commandPool, &result)) {
            LUG_LOG.error("Window::initPresentQueue: Can't create a command pool: {}", result);
            return false;
        }
    }

    return true;
}

bool Window::initSwapchain() {
    PhysicalDeviceInfo* info = _renderer.getPhysicalDeviceInfo();

    API::Builder::Swapchain swapchainBuilder(_renderer.getDevice());
    swapchainBuilder.setPreferences(_renderer.getPreferences().swapchain);
    swapchainBuilder.setImageUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    swapchainBuilder.setImageColorSpace(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
    swapchainBuilder.setMinImageCount(3);

    // If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
    if (info->swapchain.capabilities.currentExtent.height == 0xFFFFFFFF
        && info->swapchain.capabilities.currentExtent.width == 0xFFFFFFFF) {
        swapchainBuilder.setImageExtent(
            {
                _mode.width,
                _mode.height
            });
    } else {
        swapchainBuilder.setImageExtent(
            {
                info->swapchain.capabilities.currentExtent.width,
                info->swapchain.capabilities.currentExtent.height
            });
    }

    // Find the transformation of the surface
    if (info->swapchain.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        // We prefer a non-rotated transform
        swapchainBuilder.setPreTransform(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
    } else {
        swapchainBuilder.setPreTransform(info->swapchain.capabilities.currentTransform);
    }

    swapchainBuilder.setSurface(static_cast<VkSurfaceKHR>(_surface));
    swapchainBuilder.setOldSwapchain(static_cast<VkSwapchainKHR>(_swapchain));

    // Create the swapchain
    {
        // Check if the presentation and graphics queue are the same
        const API::QueueFamily* graphicsQueueFamily = _renderer.getDevice().getQueueFamily(VK_QUEUE_GRAPHICS_BIT);
        if (graphicsQueueFamily != _presentQueueFamily) {

            if (!graphicsQueueFamily) {
                LUG_LOG.error("RendererWindow: Can't find graphics queue");
                return false;
            }

            swapchainBuilder.setQueueFamilyIndices({_presentQueueFamily->getIdx(), graphicsQueueFamily->getIdx()});
        }
        else {
            swapchainBuilder.setQueueFamilyIndices({_presentQueueFamily->getIdx()});
        }

        // Reset command buffers because they use the swapchain images
        _presentQueue->waitIdle();
        {
            uint32_t frameDataSize = (uint32_t)_swapchain.getImages().size();

            for (uint32_t i = 0; i < frameDataSize; ++i) {
                for (uint32_t j = 0; j < _framesData[i].cmdBuffers.size(); ++j) {
                    _framesData[i].cmdBuffers[j].reset();
                }
            }
        }

        VkResult result{VK_SUCCESS};
        if (!swapchainBuilder.build(_swapchain, &result)) {
            LUG_LOG.error("Window::initPresentQueue: Can't create swapchain: {}", result);
            return false;
        }

        return true;
    }
}

bool Window::initFramesData() {
    uint32_t frameDataSize = (uint32_t)_swapchain.getImages().size();

    if (_framesData.size() == frameDataSize) {
        return true;
    }

    _framesData.resize(frameDataSize);
    _acquireImageDatas.resize(frameDataSize + 1);

    API::Builder::CommandBuffer commandBufferBuilder(_renderer.getDevice(), _commandPool);
    commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    API::Builder::Semaphore semaphoreBuilder(_renderer.getDevice());

    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // All draws finished semaphore
        {
            VkResult result{VK_SUCCESS};
            if (!semaphoreBuilder.build(_framesData[i].allDrawsFinishedSemaphore, &result)) {
                LUG_LOG.error("Window::initFramesData: Can't create semaphore: {}", result);
                return false;
            }
        }

        // Image ready semaphores
        {
            _framesData[i].imageReadySemaphores.resize(_initInfo.renderViewsInitInfo.size());

            for (uint32_t j = 0; j < _initInfo.renderViewsInitInfo.size(); ++j) {
                VkResult result{VK_SUCCESS};
                if (!semaphoreBuilder.build(_framesData[i].imageReadySemaphores[j], &result)) {
                    LUG_LOG.error("Window::initFramesData: Can't create semaphore: {}", result);
                    return false;
                }
            }
        }

        // Command buffers
        {
            VkResult result{VK_SUCCESS};
            _framesData[i].cmdBuffers.resize(2); // The builder will build according to the array size.
            if (!commandBufferBuilder.build(_framesData[i].cmdBuffers, &result)) {
                LUG_LOG.error("Window::initFramesData: Can't create the command buffer: {}", result);
                return false;
            }
        }
    }

    for (uint32_t i = 0; i < frameDataSize + 1; ++i) {
        // Acquire image semaphore
        {
            VkResult result{VK_SUCCESS};
            if (!semaphoreBuilder.build(_acquireImageDatas[i].completeSemaphore, &result)) {
                LUG_LOG.error("Window::initFramesData: Can't create semaphore: {}", result);
                return false;
            }

            _acquireImageDatas[i].imageIdx = -1;
        }
    }

    return buildCommandBuffers();
}

bool Window::initOffscreenData() {
    auto& device = _renderer.getDevice();
    uint32_t frameDataSize = (uint32_t)_swapchain.getImages().size();
    Vulkan::Renderer& vkRenderer = static_cast<Vulkan::Renderer&>(_renderer);

    // Get transfer queue family and retrieve the first queue
    const API::Queue* transferQueue = nullptr;
    {
        transferQueue = device.getQueue("queue_transfer");
        if (!transferQueue) {
            LUG_LOG.error("Window::initOffscreenData: Can't find transfer queue");
            return false;
        }
    }

    // Create command buffer to change layout
    API::CommandPool commandPool;
    API::CommandBuffer cmdBuffer;
    {
        VkResult result{VK_SUCCESS};

        // Command pool
        API::Builder::CommandPool commandPoolBuilder(vkRenderer.getDevice(), *transferQueue->getQueueFamily());
        if (!commandPoolBuilder.build(commandPool, &result)) {
            LUG_LOG.error("Forward::iniWindow::initOffscreenData: Can't create the graphics command pool: {}", result);
            return false;
        }

        // Command buffer
        API::Builder::CommandBuffer commandBufferBuilder(vkRenderer.getDevice(), commandPool);
        commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        // Create the render command buffer
        if (!commandBufferBuilder.build(cmdBuffer, &result)) {
            LUG_LOG.error("Window::initOffscreenData: Can't create the command buffer: {}", result);
            return false;
        }
    }

    if (!cmdBuffer.begin()) {
        LUG_LOG.error("Window::initOffscreenData: Can't begin command buffer");
        return false;
    }

    // Create fence for queue submit synchronisation
    API::Fence fence;
    {
        VkResult result{VK_SUCCESS};
        API::Builder::Fence fenceBuilder(vkRenderer.getDevice());

        if (!fenceBuilder.build(fence, &result)) {
            LUG_LOG.error("Window::initOffscreenData: Can't create render fence: {}", result);
            return false;
        }
    }

    API::Builder::DeviceMemory deviceMemoryBuilder(device);
    deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // Create offscreen images
    {
        _sceneOffscreenImages.resize(frameDataSize);
        _glowOffscreenImages.resize(frameDataSize);
        API::Builder::Image imageBuilder(device);

        imageBuilder.setUsage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        imageBuilder.setPreferedFormats({_swapchain.getFormat().format});
        imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        imageBuilder.setQueueFamilyIndices({ transferQueue->getQueueFamily()->getIdx() });
        imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);

        VkExtent3D extent{
            /* extent.width */ _swapchain.getExtent().width,
            /* extent.height */ _swapchain.getExtent().height,
            /* extent.depth */ 1
        };
        imageBuilder.setExtent(extent);

        for (uint8_t i = 0; i < frameDataSize; ++i) {
            VkResult result{VK_SUCCESS};
            if (!imageBuilder.build(_sceneOffscreenImages[i], &result) || !imageBuilder.build(_glowOffscreenImages[i], &result)) {
                LUG_LOG.error("Window::initOffscreenData: Can't create offscreen image: {}", result);
                return false;
            }

            if (!deviceMemoryBuilder.addImage(_sceneOffscreenImages[i]) || !deviceMemoryBuilder.addImage(_glowOffscreenImages[i])) {
                LUG_LOG.error("Window::initOffscreenData: Can't add offscreen image to device memory");
                return false;
            }

            // Change images layout
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &_sceneOffscreenImages[i];

                // Scene image
                cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);

                // Glow image
                pipelineBarrier.imageMemoryBarriers[0].image = &_glowOffscreenImages[i];
                cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
            }
        }
    }

    // Create offscreen image memory
    {
        VkResult result{VK_SUCCESS};
        if (!deviceMemoryBuilder.build(_offscreenImagesMemory, &result)) {
            LUG_LOG.error("Vulkan::Texture::build: Can't create buffer device memory: {}", result);
            return false;
        }
    }

    // Create offscreen image views
    {
        _sceneOffscreenImagesViews.resize(frameDataSize);
        _glowOffscreenImagesViews.resize(frameDataSize);
        for (uint8_t i = 0; i < frameDataSize; ++i) {
            // Scene image
            {
                VkResult result{VK_SUCCESS};
                API::Builder::ImageView imageViewBuilder(device, _sceneOffscreenImages[i]);

                imageViewBuilder.setFormat(_swapchain.getFormat().format);

                if (!imageViewBuilder.build(_sceneOffscreenImagesViews[i], &result)) {
                    LUG_LOG.error("Window::initOffscreenData: Can't create offscreen image view: {}", result);
                    return false;
                }
            }

            // Glow image
            {
                VkResult result{VK_SUCCESS};
                API::Builder::ImageView imageViewBuilder(device, _glowOffscreenImages[i]);

                imageViewBuilder.setFormat(_swapchain.getFormat().format);

                if (!imageViewBuilder.build(_glowOffscreenImagesViews[i], &result)) {
                    LUG_LOG.error("Window::initOffscreenData: Can't create offscreen image view: {}", result);
                    return false;
                }
            }
        }
    }

    if (!cmdBuffer.end()) {
        LUG_LOG.error("Window::initOffscreenData: Can't end command buffer");
        return false;
    }

    // Submit queue
    if (!transferQueue->submit(
        cmdBuffer,
        {},
        {},
        {},
        static_cast<VkFence>(fence)
    )) {
        LUG_LOG.error("Window::initOffscreenData Can't submit work to graphics queue");
        return false;
    }

    if (!fence.wait() || !transferQueue->waitIdle()) {
        LUG_LOG.error("Window::initOffscreenData: Can't wait fence");
        return false;
    }

    cmdBuffer.destroy();
    commandPool.destroy();

    return _bloomPass.init();
}

bool Window::buildBeginCommandBuffer() {
    uint32_t frameDataSize = (uint32_t)_swapchain.getImages().size();

    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // Build command buffer
        API::CommandBuffer& cmdBuffer = _framesData[i].cmdBuffers[0];

        if (!cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT)) {
            return false;
        }

        // Presentation to dst optimal
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &_swapchain.getImages()[i];

            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }

        // Clear the image to black
        {
            VkClearColorValue clearColor;
            std::memset(&clearColor, 0, sizeof(clearColor));

            const VkImageSubresourceRange range{
                /* range.aspectMask */ VK_IMAGE_ASPECT_COLOR_BIT,
                /* range.baseMipLevel */ 0,
                /* range.levelCount */ 1,
                /* range.baseArrayLayer*/ 0,
                /* range.layerCount */ 1
            };

            // TODO: Put this method in CommandBuffer
            vkCmdClearColorImage(
                static_cast<VkCommandBuffer>(cmdBuffer),
                static_cast<VkImage>(_swapchain.getImages()[i]),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                &clearColor,
                1,
                &range
            );
        }

        // Dst optimal to color attachment optimal
        {
            API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
            pipelineBarrier.imageMemoryBarriers.resize(1);
            pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            pipelineBarrier.imageMemoryBarriers[0].image = &_swapchain.getImages()[i];

            cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);
        }

        if (!cmdBuffer.end()) {
            return false;
        }
    }

    return true;
}

bool Window::buildEndCommandBuffer() {
    uint32_t frameDataSize = (uint32_t)_swapchain.getImages().size();

    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // Build command buffer image color attachment to present
        API::CommandBuffer& cmdBuffer = _framesData[i].cmdBuffers[1];

        if (!cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT)) {
            return false;
        }

        API::CommandBuffer::CmdPipelineBarrier pipelineBarrier{};
        pipelineBarrier.imageMemoryBarriers.resize(1);
        pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        pipelineBarrier.imageMemoryBarriers[0].image = &_swapchain.getImages()[i];

        cmdBuffer.pipelineBarrier(pipelineBarrier, VK_DEPENDENCY_BY_REGION_BIT);

        if (!cmdBuffer.end()) {
            return false;
        }
    }

    return true;
}

bool Window::buildCommandBuffers() {
    return buildBeginCommandBuffer() && buildEndCommandBuffer();
}

bool Window::init(Window::InitInfo& initInfo) {
    _initInfo = std::move(initInfo);

    // Init the window
    if (!::lug::Window::Window::init(_initInfo.windowInitInfo)) {
        return false;
    }

    if (_initInfo.renderViewsInitInfo.size() == 0) {
        _initInfo.renderViewsInitInfo.push_back({
            {                                                   // viewport
                {                                               // offset
                    0.0f,                                       // x
                    0.0f                                        // y
                },

                {                                               // extent
                    1.0f,                                       // width
                    1.0f                                        // height
                },

                0.0f,                                           // minDepth
                1.0f                                            // maxDepth
            },
            {                                                   // scissor
                {                                               // offset
                    0.0f,                                       // x
                    0.0f                                        // y
                },
                {                                               // extent
                    1.0f,                                       // width
                    1.0f                                        // height
                }
            },
            nullptr                                             // camera
        });
    }

    _threadPool = std::make_unique<lug::System::ThreadPool>(static_cast<uint8_t>(_renderViews.size()));

    return true;
}

bool Window::initRender() {
    if (!(initSurface() && initSwapchainCapabilities() && initPresentQueue() && initSwapchain() && initOffscreenData() && initFramesData())) {
        return false;
    }

    for (auto& renderViewInitInfo : _initInfo.renderViewsInitInfo) {
        if (!createView(renderViewInitInfo)) {
            return false;
        }
    }

    return true;
}

void Window::destroyRender() {
    if (_presentQueue) {
        _presentQueue->waitIdle();
    }

    for (auto& renderView : _renderViews) {
        renderView->destroy();
    }

    _renderViews.clear();

    _swapchain.destroy();
    _surface.destroy();

    _framesData.clear();

    _acquireImageDatas.clear();

    _commandPool.destroy();

    if (_isGuiInitialized == true) {
        _guiInstance.destroy();
    }

    _bloomPass.destroy();
}

} // Render
} // Vulkan
} // Graphics
} // lug
