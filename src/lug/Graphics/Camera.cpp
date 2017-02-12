#include <lug/Graphics/RenderView.hpp>
#include <lug/Graphics/Camera.hpp>

namespace lug {
namespace Graphics {

Camera::Camera(const std::string& name) : Node(name), _fov(45.0f) {
}

void Camera::update(const RenderView* renderView) {
    if (_needUpdateProj) {
        _projMatrix = Math::Geometry::perspective(
            Math::Geometry::radians(_fov),
            renderView->getViewport().getRatio(),
            _near, _far);
    }
}

void Camera::needUpdateProj() {
    _needUpdateProj = true;
    _dirty = true;
}

} // Graphics
} // lug
