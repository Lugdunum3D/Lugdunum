#include <lug/Graphics/SceneNode.hpp>
#include <lug/Graphics/RenderQueue.hpp>

namespace lug {
namespace Graphics {

SceneNode::SceneNode(const std::string& name) : _name(name) {}

SceneNode* SceneNode::getSceneNode(const std::string& name) {
    if (name == _name) {
        return this;
    }

    for (const auto& child : _children) {
        SceneNode* tmp = child->getSceneNode(name);

        if (tmp) {
            return tmp;
        }
    }

    return nullptr;
}

const SceneNode* SceneNode::getSceneNode(const std::string& name) const {
    if (name == _name) {
        return this;
    }

    for (const auto& child : _children) {
        SceneNode* tmp = child->getSceneNode(name);

        if (tmp) {
            return tmp;
        }
    }

    return nullptr;
}

void SceneNode::attachChild(std::unique_ptr<SceneNode> child) {
    child->_parent = this;
    _children.push_back(std::move(child));
}

void SceneNode::attachMovableObject(std::unique_ptr<MovableObject> movableObject) {
    _movableObjects.push_back(std::move(movableObject));
}

void SceneNode::fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const {
    for (const auto& child : _children) {
        child->fetchVisibleObjects(renderView, camera, renderQueue);
    }

    for (const auto& object : _movableObjects) {
        renderQueue.addMovableObject(object.get());
    }
}

void SceneNode::translate(const Math::Vector<3, float>& position) {
    _position += position;
}

void SceneNode::rotate(const Math::Vector<3, float>& rotation, float amount) {
    Math::Quaternion<float> quat(amount, rotation);

    _rotation = _rotation * quat;
}

void SceneNode::scale(const Math::Vector<3, float>& scale) {
    (void)scale;
    // TODO: Uncomment this when vectors could be multiplied by vectors of same size
    //_scale = _scale * scale;
}

void SceneNode::update() {
    if (_parent) {
        // TODO: Uncomment this when vectors could be multiplied by vectors of same size
/*        _absolutePosition = _parent->getAbsolutePosition() * _position;
        _absoluteRotation = _parent->getAbsoluteRotation() * _rotation;
        _absoluteScale = _parent->getAbsoluteScale() * _scale;*/
    }
    else {
        _absolutePosition = _position;
        _absoluteRotation = _rotation;
        _absoluteScale = _scale;
    }

    // TODO: construct scale and translation matrix
    // TODO: apply transformations to _transform

    for (const auto& child : _children) {
        child->update();
    }
}

} // Graphics
} // lug
