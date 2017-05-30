#include <lug/Graphics/Vulkan/Render/View.hpp>

#include <algorithm>

#include <lug/Graphics/Render/Queue.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Render/Technique/Forward.hpp>
#include <lug/Graphics/Vulkan/Render/Window.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

View::View(const Renderer& renderer, const ::lug::Graphics::Render::Target* renderTarget) : ::lug::Graphics::Render::View(renderTarget), _renderer(renderer) {}

bool View::init(
    View::InitInfo& initInfo,
    const API::Queue* presentQueue,
    API::DescriptorPool* descriptorPool,
    const std::vector<API::ImageView>& imageViews) {
    ::lug::Graphics::Render::View::init(initInfo);

    if (_info.renderTechniqueType == lug::Graphics::Render::Technique::Type::Forward) {
        _renderTechnique = std::make_unique<Render::Technique::Forward>(_renderer, *this);
    }

    if (_renderTechnique && !_renderTechnique->init(descriptorPool, imageViews)) {
        LUG_LOG.warn("View::init: Failed to init render technique");
        return false;
    }

    _drawCompleteSemaphores.resize(imageViews.size());

    API::Builder::Semaphore semaphoreBuilder(_renderer.getDevice());

    // Work complete semaphores
    for (uint32_t i = 0; i < imageViews.size(); ++i) {
        VkResult result{VK_SUCCESS};
        if (!semaphoreBuilder.build(_drawCompleteSemaphores[i], &result)) {
            LUG_LOG.error("View::init: Can't create semaphore: {}", result);
            return false;
        }
    }

    _presentQueue = presentQueue;

    return true;
}

bool View::render(const API::Semaphore& imageReadySemaphore, uint32_t currentImageIndex) {
    if (!_camera) {
        LUG_LOG.warn("View::render: Attempt to render with no camera attached");
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
