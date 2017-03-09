#include <lug/Graphics/Scene/MovableCamera.hpp>
#include <lug/Graphics/Render/Camera.hpp>
#include <lug/Graphics/Scene/Node.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

MovableCamera::MovableCamera(const std::string& name, Render::Camera* camera) : MovableObject(name, MovableObject::Type::Camera) {
    if (camera) {
        attachCamera(camera);
    }
}

void MovableCamera::setParent(Node* parent) {
    MovableObject::setParent(parent);

    if (!_camera) {
        return;
    }

    _camera->setParent(_parent);
}

bool MovableCamera::attachCamera(Render::Camera* camera) {
    detachCamera();

    // The camera already have a parent
    if (camera->getParent()) {
        return false;
    }

    _camera = camera;
    _camera->setParent(_parent);

    return true;
}

void MovableCamera::detachCamera() {
    if (!_camera) {
        return;
    }

    _camera->setParent(nullptr);
}

void MovableCamera::needUpdate() {
    if (!_camera) {
        return;
    }

    _camera->needUpdate();
}

} // Scene
} // Graphics
} // lug
