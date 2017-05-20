#include <cstring>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/Window.hpp>
#include <lug/Graphics/Vulkan/Render/View.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/System/Debug.hpp>
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

Window::Window(Renderer& renderer) : _renderer(renderer) {}

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

        return true;
    }

    return false;
}

bool Window::beginFrame() {
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

    while (_swapchain.isOutOfDate() || !_swapchain.getNextImage(&_currentImageIndex, static_cast<VkSemaphore>(acquireImageData->completeSemaphore))) {
        if (_swapchain.isOutOfDate()) {
            if (!initSwapchainCapabilities() || !initSwapchain() || !buildCommandBuffers()) {
                return false;
            }

            for (auto& renderView: _renderViews) {
                View* renderView_ = static_cast<View*>(renderView.get());

                if (!renderView_->getRenderTechnique()->initDepthBuffers(_swapchain.getImagesViews()) ||
                    !renderView_->getRenderTechnique()->initFramebuffers(_swapchain.getImagesViews())) {
                    return false;
                }
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

    std::vector<VkSemaphore> imageReadyVkSemaphores;
    imageReadyVkSemaphores.reserve(frameData.imageReadySemaphores.size());

    std::transform(
        frameData.imageReadySemaphores.begin(),
        frameData.imageReadySemaphores.end(),
        std::back_inserter(imageReadyVkSemaphores),
        [] (const API::Semaphore& semaphore) {
                        return static_cast<VkSemaphore>(semaphore);
                    }
        );

    return _presentQueue->submit(
        cmdBuffer,
        imageReadyVkSemaphores,
        {static_cast<VkSemaphore>(acquireImageData->completeSemaphore)},
        {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT}
        );
}

bool Window::endFrame() {
    FrameData& frameData = _framesData[_currentImageIndex];

    API::CommandBuffer& cmdBuffer = frameData.cmdBuffers[1];
    std::vector<VkSemaphore> waitSemaphores(_renderViews.size());
    uint32_t i = 0;

    for (auto& renderView: _renderViews) {
        View* renderView_ = static_cast<View*>(renderView.get());

        // Render views with no camera don't signal the semaphore as they don't draw
        if (renderView_->getCamera()) {
            waitSemaphores[i++] = static_cast<VkSemaphore>(renderView_->getDrawCompleteSemaphore(_currentImageIndex));
        }
    }

    // Update wait semaphores vector size,
    // it could be != from _renderViews.size(), if some render views has no camera
    if (waitSemaphores.size() != i) {
        waitSemaphores.resize(i);
    }
    std::vector<VkPipelineStageFlags> waitDstStageMasks(waitSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);

    return _presentQueue->submit(
        cmdBuffer,
        {static_cast<VkSemaphore>(frameData.allDrawsFinishedSemaphore)},
        waitSemaphores, waitDstStageMasks
    ) && _swapchain.present(_presentQueue, _currentImageIndex, static_cast<VkSemaphore>(frameData.allDrawsFinishedSemaphore));
}

lug::Graphics::Render::View* Window::createView(lug::Graphics::Render::View::InitInfo& initInfo) {
    std::unique_ptr<View> renderView = std::make_unique<View>(_renderer, this);

    if (!renderView->init(initInfo, &_renderer.getDevice(), _presentQueue, _descriptorPool.get(), _swapchain.getImagesViews())) {
        return nullptr;
    }

    _renderViews.push_back(std::move(renderView));

    return _renderViews.back().get();
}

bool Window::render() {
    FrameData& frameData = _framesData[_currentImageIndex];
    uint32_t i = 0;

    for (auto& renderView: _renderViews) {
        if (!static_cast<View*>(renderView.get())->render(frameData.imageReadySemaphores[i++], _currentImageIndex)) {
            return false;
        }
    }

    return true;
}

std::unique_ptr<Window>

Window::create(Renderer& renderer, Window::InitInfo& initInfo) {
    std::unique_ptr<Window> window(new Window(renderer));

    if (!window->init(initInfo)) {
        return nullptr;
    }

    return window;
}

bool Window::initDescriptorPool() {
    // Create descriptor pool
    // This is in Window because we need to know the number of RenderTechnique (Actually the number of render views)
    // Each RenderTechnique has got 50 descriptors for the lights and 1 for the camera

    VkDescriptorPoolSize poolSize[] = {
        // Dynamic uniform buffers descriptors (1 for camera and 1 for lights in ForwardRenderTechnique)
        {
            poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
            poolSize[0].descriptorCount = (uint32_t)_initInfo.renderViewsInitInfo.size() * 3 * 2
        }
    };

    VkDescriptorPoolCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0, // Use VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT to individually free descritors sets
        createInfo.maxSets = (uint32_t)_initInfo.renderViewsInitInfo.size() * (1 + 1) * 3, // Only ForwardRenderTechnique has 1 descriptor sets (for lights) and 1 (for the camera)
        createInfo.poolSizeCount = 1,
        createInfo.pPoolSizes = poolSize
    };

    VkDescriptorPool descriptorPool{VK_NULL_HANDLE};

    VkResult result = vkCreateDescriptorPool(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &descriptorPool);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan", "Can't create the descriptor pool: {}", result);
        return false;
    }

    _descriptorPool = std::make_unique<API::DescriptorPool>(descriptorPool, &_renderer.getDevice());

    return true;
}

/**
 * Init surface
 * @return Success
 */
bool Window::initSurface() {
#if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
    VkWin32SurfaceCreateInfoKHR createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.hinstance = _impl->getHinstance(),
        createInfo.hwnd = _impl->getHandle()
    };

    VkResult result = vkCreateWin32SurfaceKHR(static_cast<VkInstance>(_renderer.getInstance()), &createInfo, nullptr, &_surface);
#elif defined(LUG_SYSTEM_LINUX) // Linux surface
    VkXlibSurfaceCreateInfoKHR createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.dpy = _impl->getDisplay(),
        createInfo.window = _impl->getWindow()
    };

    VkResult result = vkCreateXlibSurfaceKHR(static_cast<VkInstance>(_renderer.getInstance()), &createInfo, nullptr, &_surface);
#elif defined(LUG_SYSTEM_ANDROID) // Android Surface
    VkAndroidSurfaceCreateInfoKHR createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.window = _impl->getWindow()
    };

    VkResult result = vkCreateAndroidSurfaceKHR(static_cast<VkInstance>(_renderer.getInstance()), &createInfo, nullptr, &_surface);
#else
    #error "Window::initSurface Unknow platform"
#endif

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererWindow", "Can't initialize surface: {}", result);
        return false;
    }

    return true;
}

bool Window::initSwapchainCapabilities() {
    VkResult result;
    PhysicalDeviceInfo* info = _renderer.getPhysicalDeviceInfo();

    LUG_ASSERT(info != nullptr, "PhysicalDeviceInfo cannot be null");

    // Get swapchain capabilities
    {
        result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info->handle, _surface, &info->swapchain.capabilities);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow", "Can't get surface capabilities: {}", result);
            return false;
        }

        uint32_t formatsCount = 0;
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(info->handle, _surface, &formatsCount, nullptr);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow", "Can't retrieve formats count: {}", result);
            return false;
        }

        info->swapchain.formats.resize(formatsCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(info->handle, _surface, &formatsCount, info->swapchain.formats.data());

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow", "Can't retrieve formats: {}", result);
            return false;
        }

        uint32_t presentModesCount = 0;
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(info->handle, _surface, &presentModesCount, nullptr);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow", "Can't retrieve present modes count: {}", result);
            return false;
        }

        info->swapchain.presentModes.resize(presentModesCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(info->handle, _surface, &presentModesCount, info->swapchain.presentModes.data());

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow", "Can't retrieve present modes: {}", result);
            return false;
        }
    }

    return true;
}

bool Window::initPresentQueue() {
    VkResult result;
    PhysicalDeviceInfo* info = _renderer.getPhysicalDeviceInfo();

    LUG_ASSERT(info != nullptr, "PhysicalDeviceInfo cannot be null");

    // Get present queues
    {
        for (auto& queue : _renderer.getQueues()) {
            VkBool32 supported = 0;
            result = vkGetPhysicalDeviceSurfaceSupportKHR(info->handle, queue.getFamilyIdx(), _surface, &supported);

            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererWindow: Can't check if queue supports presentation: {}", result);
                return false;
            }

            queue.supportsPresentation(!!supported);
        }
    }

    return true;
}

bool Window::initSwapchain() {
    VkResult result;
    PhysicalDeviceInfo* info = _renderer.getPhysicalDeviceInfo();

    LUG_ASSERT(info != nullptr, "PhysicalDeviceInfo cannot be null");

    // TODO: Find a way to put Preferences elsewhere
    Renderer::Preferences::Swapchain& swapchainPreferences = _renderer.getPreferences().swapchain;

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
    VkSurfaceFormatKHR swapchainFormat{
        swapchainFormat.format = VK_FORMAT_MAX_ENUM,
        swapchainFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    };
    VkCompositeAlphaFlagBitsKHR compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(0);
    uint32_t minImageCount = 3;
    VkExtent2D extent{};
    VkSurfaceTransformFlagsKHR transform{};

    // Check requirements for swapchain
    {
        // Check the present mode
        {
            for (auto presentMode : swapchainPreferences.presentModes) {
                if (std::find(info->swapchain.presentModes.begin(), info->swapchain.presentModes.end(), presentMode) != info->swapchain.presentModes.end()) {
                    LUG_LOG.info("RendererWindow", "Use present mode {}", API::RTTI::toStr(presentMode));

                    swapchainPresentMode = presentMode;
                    break;
                }
            }

            if (swapchainPresentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
                LUG_LOG.error("RendererWindow", "Missing present mode supported by Lugdunum");
                return false;
            }
        }

        // Check the formats
        {
            for (auto format : swapchainPreferences.formats) {
                if (std::find_if(info->swapchain.formats.begin(), info->swapchain.formats.end(), [&swapchainFormat, &format](const VkSurfaceFormatKHR& lhs) {
                    return lhs.colorSpace == swapchainFormat.colorSpace && format == lhs.format;
                }) != info->swapchain.formats.end()) {
                    LUG_LOG.info("RendererWindow", "Use format {}", API::RTTI::toStr(format));

                    swapchainFormat.format = format;
                    break;
                }
            }

            if (swapchainFormat.format == VK_FORMAT_MAX_ENUM) {
                LUG_LOG.error("RendererWindow", "Missing swapchain format supported by Lugdunum");
                return false;
            }
        }

        // Check composite alpha
        {
            for (auto compositeAlphaPreferency : swapchainPreferences.compositeAlphas) {
                if (info->swapchain.capabilities.supportedCompositeAlpha & compositeAlphaPreferency) {
                    LUG_LOG.info("RendererWindow", "Use composite alpha {}", API::RTTI::toStr(compositeAlphaPreferency));

                    compositeAlpha = compositeAlphaPreferency;
                    break;
                }
            }

            if (!compositeAlpha) {
                LUG_LOG.error("RendererWindow", "Missing composite alpha supported by Lugdunum");
                return false;
            }
        }

        // Check image counts
        if (info->swapchain.capabilities.maxImageCount > 0 && info->swapchain.capabilities.maxImageCount < minImageCount) {
            LUG_LOG.error("RendererWindow", "Not enough images ({} required), found {}", minImageCount, info->swapchain.capabilities.maxImageCount);
            return false;
        }

        // If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
        if (info->swapchain.capabilities.currentExtent.height == 0xFFFFFFFF
            && info->swapchain.capabilities.currentExtent.width == 0xFFFFFFFF) {
            extent.height = _mode.height;
            extent.width = _mode.width;
        } else {
            extent.height = info->swapchain.capabilities.currentExtent.height;
            extent.width = info->swapchain.capabilities.currentExtent.width;
        }

        // Find the transformation of the surface
        if (info->swapchain.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
            // We prefer a non-rotated transform
            transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        } else {
            transform = info->swapchain.capabilities.currentTransform;
        }
    }

    // Create the swapchain
    {
        VkSwapchainCreateInfoKHR createInfo{
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            createInfo.pNext = nullptr,
            createInfo.flags = 0,
            createInfo.surface = _surface,
            createInfo.minImageCount = minImageCount,
            createInfo.imageFormat = swapchainFormat.format,
            createInfo.imageColorSpace = swapchainFormat.colorSpace,
            {}, // createInfo.imageExtent
            createInfo.imageArrayLayers = 1,
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            createInfo.queueFamilyIndexCount = 0,
            createInfo.pQueueFamilyIndices = nullptr,
            createInfo.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(transform),
            createInfo.compositeAlpha = compositeAlpha,
            createInfo.presentMode = swapchainPresentMode,
            createInfo.clipped = VK_TRUE,
            createInfo.oldSwapchain = static_cast<VkSwapchainKHR>(_swapchain)
        };

        // Thank you clang
        createInfo.imageExtent.width = extent.width;
        createInfo.imageExtent.height = extent.height;

        std::vector<uint32_t> queueFamilyIndices(1);

        _presentQueue = _renderer.getQueue(0, true);

        if (!_presentQueue) {
            LUG_LOG.error("RendererWindow", "Can't find presentation queue");
            return false;
        }

        queueFamilyIndices[0] = _presentQueue->getFamilyIdx();

        // Check if the presentation and graphics queue are the same
        if (!_renderer.isSameQueue(0, true, VK_QUEUE_GRAPHICS_BIT, false)) {
            const API::Queue* graphicsQueue = _renderer.getQueue(VK_QUEUE_GRAPHICS_BIT, false);

            if (!graphicsQueue) {
                LUG_LOG.error("RendererWindow", "Can't find graphics queue");
                return false;
            }

            queueFamilyIndices.push_back(graphicsQueue->getFamilyIdx());
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        }

        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();

        VkSwapchainKHR swapchainKHR;
        result = vkCreateSwapchainKHR(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &swapchainKHR);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("RendererWindow", "Can't initialize swapchain: {}", result);
            return false;
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

        _swapchain = API::Swapchain(swapchainKHR, &_renderer.getDevice(), swapchainFormat, extent);

        return _swapchain.init();
    }
}

bool Window::initFramesData() {
    uint32_t frameDataSize = (uint32_t)_swapchain.getImages().size();

    if (_framesData.size() == frameDataSize) {
        return true;
    }

    _framesData.resize(frameDataSize);
    _acquireImageDatas.resize(frameDataSize + 1);

    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // All draws finished semaphore
        {
            VkSemaphore semaphore;
            VkSemaphoreCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = 0
            };
            VkResult result = vkCreateSemaphore(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &semaphore);

            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan", "Can't create swapchain semaphore: {}", result);
                return false;
            }

            _framesData[i].allDrawsFinishedSemaphore = API::Semaphore(semaphore, &_renderer.getDevice());
        }

        // Image ready semaphores
        {
            VkSemaphoreCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = 0
            };
            _framesData[i].imageReadySemaphores.resize(_initInfo.renderViewsInitInfo.size());

            for (uint32_t j = 0; j < _initInfo.renderViewsInitInfo.size(); ++j) {
                VkSemaphore semaphore;
                VkResult result = vkCreateSemaphore(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &semaphore);

                if (result != VK_SUCCESS) {
                    LUG_LOG.error("RendererVulkan", "Can't create swapchain semaphore: {}", result);
                    return false;
                }

                _framesData[i].imageReadySemaphores[j] = API::Semaphore(semaphore, &_renderer.getDevice());
            }
        }

        // Command buffers
        _framesData[i].cmdBuffers = _renderer.getQueue(0, true)->getCommandPool().createCommandBuffers(VK_COMMAND_BUFFER_LEVEL_PRIMARY, 2);
    }

    for (uint32_t i = 0; i < frameDataSize + 1; ++i) {
        // Acquire image semaphore
        {
            VkSemaphore semaphore;
            VkSemaphoreCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = 0
            };
            VkResult result = vkCreateSemaphore(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &semaphore);

            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan", "Can't create swapchain semaphore: {}", result);
                return false;
            }

            _acquireImageDatas[i].completeSemaphore = API::Semaphore(semaphore, &_renderer.getDevice());
            _acquireImageDatas[i].imageIdx = -1;
        }
    }

    return buildCommandBuffers();
}

bool Window::buildCommandBuffers() {
    uint32_t frameDataSize = (uint32_t)_swapchain.getImages().size();

    for (uint32_t i = 0; i < frameDataSize; ++i) {
        // Build command buffer image present to color attachment
        {
            API::CommandBuffer& cmdBuffer = _framesData[i].cmdBuffers[0];

            if (!cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT )) {
                return false;
            }

            _swapchain.getImages()[i].changeLayout(
                cmdBuffer,
                0,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

            if (!cmdBuffer.end()) {
                return false;
            }
        }

        // Build command buffer image color attachment to present
        {
            API::CommandBuffer& cmdBuffer = _framesData[i].cmdBuffers[1];

            if (!cmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT )) {
                return false;
            }

            _swapchain.getImages()[i].changeLayout(
                cmdBuffer,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_ACCESS_MEMORY_READ_BIT,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

            if (!cmdBuffer.end()) {
                return false;
            }
        }
    }

    return true;
}

bool Window::init(Window::InitInfo& initInfo) {
    _initInfo = std::move(initInfo);

    // Init the window
    if (!::lug::Window::Window::init(_initInfo.windowInitInfo)) {
        return false;
    }

    if (_initInfo.renderViewsInitInfo.size() == 0) {
        _initInfo.renderViewsInitInfo.push_back({
            lug::Graphics::Render::Technique::Type::Forward,    // renderTechniqueType
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

    return initRender();
}

bool Window::initRender() {
    if (!(initDescriptorPool() && initSurface() && initSwapchainCapabilities() && initPresentQueue() && initSwapchain() && initFramesData())) {
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

    if (_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(static_cast<VkInstance>(_renderer.getInstance()), _surface, nullptr);
        _surface = VK_NULL_HANDLE;
    }

    _framesData.clear();

    _descriptorPool->destroy();

    _acquireImageDatas.clear();
}

} // Render
} // Vulkan
} // Graphics
} // lug
