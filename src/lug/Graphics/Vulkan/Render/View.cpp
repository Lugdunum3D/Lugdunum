#include <lug/Graphics/Vulkan/Render/View.hpp>
#include <lug/Graphics/Render/Queue.hpp>
#include <lug/Graphics/Vulkan/Render/Technique/Forward.hpp>
#include <lug/Graphics/Vulkan/Render/Window.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

View::View(const Renderer& renderer, const ::lug::Graphics::Render::Target* renderTarget) : ::lug::Graphics::Render::View(renderTarget), _renderer(renderer) {}

bool View::init(
    View::InitInfo& initInfo,
    const API::Device* device,
    const API::Queue* presentQueue,
    API::DescriptorPool* descriptorPool,
    const std::vector<API::ImageView>& imageViews) {
    ::lug::Graphics::Render::View::init(initInfo);

    if (_info.renderTechniqueType == lug::Graphics::Render::Technique::Type::Forward) {
        _renderTechnique = std::make_unique<Render::Technique::Forward>(_renderer, this, device);
    }

    if (_renderTechnique && !_renderTechnique->init(descriptorPool, imageViews)) {
        LUG_LOG.warn("View: Failed to init render technique");
        return false;
    }

    _drawCompleteSemaphores.resize(imageViews.size());

    for (uint32_t i = 0; i < imageViews.size(); ++i) {
        // Work complete semaphore
        {
            VkSemaphore semaphore;
            VkSemaphoreCreateInfo createInfo{
                createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                createInfo.pNext = nullptr,
                createInfo.flags = 0
            };
            VkResult result = vkCreateSemaphore(static_cast<VkDevice>(*device), &createInfo, nullptr, &semaphore);

            if (result != VK_SUCCESS) {
                LUG_LOG.error("RendererVulkan: Can't create swapchain semaphore: {}", result);
                return false;
            }

            _drawCompleteSemaphores[i] = API::Semaphore(semaphore, device);
        }
    }

    _presentQueue = presentQueue;

    return true;
}

bool View::render(const API::Semaphore& imageReadySemaphore, uint32_t currentImageIndex) {
    if (!_camera) {
        LUG_LOG.warn("View: Attempt to render with no camera attached");
        return true; // Not fatal, return success anyway
    }

    _camera->update(this);
    return _renderTechnique->render(_camera->getRenderQueue(), imageReadySemaphore, _drawCompleteSemaphores[currentImageIndex], currentImageIndex);
}

void View::destroy() {
    _renderTechnique->destroy();
}

bool View::endFrame() {
    if (_camera) {
        // Call isDirty(false) for each objects in the RenderQueue
        _camera->getRenderQueue().removeDirtyProperty();
    }

    return true;
}

VkSurfaceFormatKHR View::getFormat() const {
    return static_cast<const Render::Window*>(_renderTarget)->getSwapchain().getFormat();
}

} // Render
} // Vulkan
} // Graphics
} // lug
