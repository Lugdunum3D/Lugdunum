#include <lug/Graphics/Vulkan/Camera.hpp>
#include <lug/Graphics/Scene.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Camera::Camera(const std::string& name) : lug::Graphics::Camera(name) {}

void Camera::update(const lug::Graphics::RenderView* renderView) {
    lug::Graphics::Camera::update(renderView);
    _renderQueue.clear();

    if (_scene) {
        _scene->fetchVisibleObjects(renderView, this, _renderQueue);
    } else {
        LUG_LOG.warn("Camera: Attempt to update with no scene attached");
    }
}

} // Vulkan
} // Graphics
} // lug
