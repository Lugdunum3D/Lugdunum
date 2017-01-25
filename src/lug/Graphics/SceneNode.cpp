#include <lug/Graphics/SceneNode.hpp>
#include <lug/Graphics/RenderQueue.hpp>

namespace lug {
namespace Graphics {

SceneNode::SceneNode(const std::string& name) : _name(name) {}

SceneNode* SceneNode::getSceneNode(const std::string& name) {
    if (name == _name) {
        return this;
    }

    for (const auto& child : children) {
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

    for (const auto& child : children) {
        SceneNode* tmp = child->getSceneNode(name);

        if (tmp) {
            return tmp;
        }
    }

    return nullptr;
}

void SceneNode::attachChild(std::unique_ptr<SceneNode> child) {
    child->parent = this;
    children.push_back(std::move(child));
}

void SceneNode::attachMovableObject(std::unique_ptr<MovableObject> movableObject) {
    _movableObjects.push_back(std::move(movableObject));
}

void SceneNode::fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const {
    for (const auto& child : children) {
        child->fetchVisibleObjects(renderView, camera, renderQueue);
    }

    for (const auto& object : _movableObjects) {
        renderQueue.addMovableObject(object.get());
    }
}

} // Graphics
} // lug
