#include <lug/Graphics/RenderView.hpp>
#include <lug/Graphics/Camera.hpp>

namespace lug {
namespace Graphics {

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

} // Graphics
} // lug
