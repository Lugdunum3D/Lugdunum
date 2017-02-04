#include <lug/Graphics/MovableCamera.hpp>
#include <lug/Graphics/Camera.hpp>
#include <lug/Graphics/SceneNode.hpp>

namespace lug {
namespace Graphics {

MovableCamera::MovableCamera(const std::string& name, Camera* camera) : MovableObject(name, MovableObject::Type::CAMERA) {
    if (camera) {
        attachCamera(camera);
    }
}

void MovableCamera::setParent(SceneNode* parent) {
    MovableObject::setParent(parent);

    if (!_camera) {
        return;
    }

    _camera->setParent(_parent);
}

bool MovableCamera::attachCamera(Camera* camera) {
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

} // Graphics
} // lug
