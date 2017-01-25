#include <lug/Graphics/Vulkan/Camera.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

void Camera::update(const RenderView* renderView) {
    _renderQueue.clear();

    if (_scene) {
        _scene->fetchVisibleObjects(renderView, this, _renderQueue);
    }
    else {
        LUG_LOG.warn("Camera: Attempt to update with no scene attached");
    }
}

} // Vulkan
} // Graphics
} // lug
