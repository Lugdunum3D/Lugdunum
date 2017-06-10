#include <lug/Graphics/Builder/Camera.hpp>

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Render/Camera/Orthographic.hpp>
#include <lug/Graphics/Render/Camera/Perspective.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Camera::Camera(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<Render::Camera::Camera> Camera::build() {
    std::unique_ptr<Resource> resource{nullptr};

    switch(_type) {
        case Camera::Type::Perspective: {
            resource = std::unique_ptr<Resource>(new Render::Camera::Perspective(_name));
            Render::Camera::Perspective* camera = static_cast<Render::Camera::Perspective*>(resource.get());

            camera->setAspectRatio(_aspectRatio);
            camera->setFovY(_fovy);

            camera->setZNear(_znear);
            camera->setZFar(_zfar);

            break;
        }

        case Camera::Type::Orthographic: {
            resource = std::unique_ptr<Resource>(new Render::Camera::Orthographic(_name));
            Render::Camera::Orthographic* camera = static_cast<Render::Camera::Orthographic*>(resource.get());

            camera->setXMag(_xmag);
            camera->setYMag(_ymag);

            camera->setZNear(_znear);
            camera->setZFar(_zfar);

            break;
        }
    }

    return resource ? _renderer.getResourceManager()->add<::lug::Graphics::Render::Camera::Camera>(std::move(resource)) : nullptr;
}

} // Builder
} // Graphics
} // lug
