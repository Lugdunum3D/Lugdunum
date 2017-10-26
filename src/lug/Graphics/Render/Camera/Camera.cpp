#include <lug/Graphics/Render/Camera/Camera.hpp>

#include <lug/Graphics/Render/View.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Render {
namespace Camera {

Camera::Camera(const std::string& name) : Resource(Resource::Type::Camera, name) {}

void Camera::lookAt(const Math::Vec3f& targetPosition, const Math::Vec3f& up, Node::TransformSpace space) {
    if (_parent) {
        _parent->lookAt(targetPosition, {0.0f, 0.0f, -1.0f}, up, space);
    } else {
        LUG_LOG.warn("Camera: Attempt to use lookAt on a Camera without parent");
    }
}

void Camera::update(const Renderer& renderer, const ::lug::Graphics::Render::View& renderView, Queue& renderQueue) {
    if (_parent) {
        _parent->getScene().fetchVisibleObjects(renderer, renderView, *this, renderQueue);
    }
}

void Camera::updateView() {
    if (_parent) {
        _viewMatrix = _parent->getTransform().inverse();
    }
    else {
        _viewMatrix = Math::Mat4x4f::identity();
    }
    _needUpdateView = false;
}

} // Camera
} // Render
} // Graphics
} // lug
