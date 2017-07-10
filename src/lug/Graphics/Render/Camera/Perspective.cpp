#include <lug/Graphics/Render/Camera/Perspective.hpp>

#include <lug/Graphics/Render/View.hpp>
#include <lug/Math/Geometry/Transform.hpp>

namespace lug {
namespace Graphics {
namespace Render {
namespace Camera {

Perspective::Perspective(const std::string& name) : Camera(name) {}

void Perspective::updateProj() {
    if (!_renderView) {
        return;
    }

    _projMatrix = Math::Geometry::perspective(
        Math::Geometry::radians(_fovy),
        _aspectRatio == 0.0f ? _renderView->getViewport().getRatio() : _aspectRatio,
        _znear,
        _zfar
    );

    _needUpdateProj = false;
}

} // Camera
} // Render
} // Graphics
} // lug
