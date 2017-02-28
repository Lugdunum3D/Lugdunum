#include <lug/Graphics/Vulkan/RenderView.hpp>
#include <lug/Graphics/RenderQueue.hpp>
#include <lug/Graphics/Vulkan/ForwardRenderTechnique.hpp>
#include <lug/Graphics/Vulkan/RenderWindow.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

RenderView::RenderView(const Renderer& renderer, const RenderTarget* renderTarget) : lug::Graphics::RenderView(renderTarget), _renderer(renderer) {}

bool RenderView::init(RenderView::InitInfo& initInfo,
                        const Device* device,
                        Queue* presentQueue,
                        DescriptorPool* descriptorPool,
                        const std::vector<std::unique_ptr<ImageView> >& imageViews) {
    lug::Graphics::RenderView::init(initInfo);

    if (_info.renderTechniqueType == lug::Graphics::RenderTechnique::Type::Forward) {
        _renderTechnique = std::make_unique<ForwardRenderTechnique>(_renderer, this, device, presentQueue);
    }

    if (_renderTechnique && !_renderTechnique->init(descriptorPool, imageViews)) {
        LUG_LOG.warn("RenderView: Failed to init render technique");
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

            _drawCompleteSemaphores[i] = Semaphore(semaphore, device);
        }
    }

    _presentQueue = presentQueue;

    return true;
}

bool RenderView::render(const Semaphore& imageReadySemaphore, uint32_t currentImageIndex) {
    if (!_camera) {
        LUG_LOG.warn("RenderView: Attempt to render with no camera attached");
        return true; // Not fatal, return success anyway
    }
    _camera->update(this);
    return _renderTechnique->render(_camera->getRenderQueue(), imageReadySemaphore, _drawCompleteSemaphores[currentImageIndex], currentImageIndex);
}

void RenderView::destroy() {
    _renderTechnique->destroy();
}

bool RenderView::endFrame() {
    if (_camera) {
        // Call isDirty(false) for each objects in the RenderQueue
        _camera->getRenderQueue().removeDirtyProperty();
    }
    return true;
}

VkSurfaceFormatKHR RenderView::getFormat() const {
    return static_cast<const RenderWindow*>(_renderTarget)->getSwapchain().getFormat();
}

} // Vulkan
} // Graphics
} // lug
