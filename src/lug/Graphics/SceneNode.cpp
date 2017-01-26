#include <lug/Graphics/SceneNode.hpp>
#include <lug/Graphics/RenderQueue.hpp>

namespace lug {
namespace Graphics {

SceneNode::SceneNode(const std::string& name) : Node(name) {}

void SceneNode::attachMovableObject(std::unique_ptr<MovableObject> movableObject) {
    movableObject->setParent(this);
    _movableObjects.push_back(std::move(movableObject));
}

void SceneNode::fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const {
    for (const auto& child : _children) {
        static_cast<const SceneNode*>(child.get())->fetchVisibleObjects(renderView, camera, renderQueue);
    }

    for (const auto& object : _movableObjects) {
        renderQueue.addMovableObject(object.get());
    }
}

void SceneNode::needUpdate() {
    Node::needUpdate();

    for (auto& object : _movableObjects) {
        object->needUpdate();
    }
}

} // Graphics
} // lug
