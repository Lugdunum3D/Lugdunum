#include <lug/Graphics/Vulkan/Render/Camera.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Camera::Camera(const std::string& name) : ::lug::Graphics::Render::Camera(name) {}

void Camera::update(const ::lug::Graphics::Render::View* renderView) {
    _renderQueue.clear();

    if (_scene) {
        _scene->fetchVisibleObjects(renderView, this, _renderQueue);
    } else {
        LUG_LOG.warn("Camera", "Attempt to update with no scene attached");
    }
}

} // Render
} // Vulkan
} // Graphics
} // lug
