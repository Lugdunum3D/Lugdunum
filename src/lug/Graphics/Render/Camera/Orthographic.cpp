#include <lug/Graphics/Render/Camera/Orthographic.hpp>

#include <lug/Graphics/Render/View.hpp>
#include <lug/Math/Geometry/Transform.hpp>

namespace lug {
namespace Graphics {
namespace Render {
namespace Camera {

Orthographic::Orthographic(const std::string& name) : Camera(name) {}

void Orthographic::updateProj() {
    if (!_renderView) {
        return;
    }

    float xmag = _xmag;
    float ymag = _ymag;

    const auto& viewport = _renderView->getViewport();

    if (xmag == 0.0f && ymag != 0.0f) {
        xmag = ymag / viewport.getRatio();
    } else if (xmag != 0.0f && ymag == 0.0f) {
        ymag = xmag * viewport.getRatio();
    } else if (xmag == 0.0f && ymag == 0.0f) {
        xmag = viewport.extent.width;
        ymag = viewport.extent.height;
    }

    _projMatrix = Math::Geometry::ortho(
        -xmag / 2.0f, xmag / 2.0f,
        -ymag / 2.0f, ymag / 2.0f,
        _znear,
        _zfar
    );

    _needUpdateProj = false;
}

} // Camer
} // Render
} // Graphics
} // lug
