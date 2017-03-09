#include <lug/Graphics/Render/Camera.hpp>
#include <lug/Graphics/Render/View.hpp>

namespace lug {
namespace Graphics {
namespace Render {

Camera::Camera(const std::string& name) : Node(name) {
}

void Camera::needUpdate() {
    Node::needUpdate();
    needUpdateView();
}

void Camera::needUpdateProj() {
    _needUpdateProj = true;
    _dirty = true;
}

void Camera::needUpdateView() {
    _needUpdateView = true;
    _dirty = true;
}

void Camera::lookAt(const Math::Vec3f& targetPosition, const Math::Vec3f& up, Node::TransformSpace space) {
    Node::lookAt(targetPosition, {0.0f, 0.0f, -1.0f}, up, space);
}

void Camera::updateProj() {
    if (!_renderView) {
        return;
    }

    _projMatrix = Math::Geometry::perspective(
        Math::Geometry::radians(_fov),
        _renderView->getViewport().getRatio(),
        _near, _far
    );

    _needUpdateProj = false;
}

void Camera::updateView() {
    _viewMatrix = getTransform().inverse();
    _needUpdateView = false;
}

} // Render
} // Graphics
} // lug
