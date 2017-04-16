#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/Graphics/Render/Queue.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

Node::Node(Scene& scene, const std::string& name) : ::lug::Graphics::Node(name), _scene(scene) {}

Node* Node::createSceneNode(const std::string& name, std::unique_ptr<MovableObject> object) {
    std::unique_ptr<Node> node = _scene.createSceneNode(name, std::move(object));

    Node* ptrNode = node.get();
    attachChild(std::move(node));
    return ptrNode;
}

void Node::attachMovableObject(std::unique_ptr<MovableObject> movableObject) {
    movableObject->setParent(this);
    _movableObjects.push_back(std::move(movableObject));
}

void Node::fetchVisibleObjects(const Render::View* renderView, const Render::Camera* camera, Render::Queue& renderQueue) const {
    for (const auto& child : _children) {
        static_cast<const Node*>(child.get())->fetchVisibleObjects(renderView, camera, renderQueue);
    }

    for (const auto& object : _movableObjects) {
        renderQueue.addMovableObject(object.get());
    }
}

void Node::needUpdate() {
    ::lug::Graphics::Node::needUpdate();

    for (auto& object : _movableObjects) {
        object->needUpdate();
    }
}

} // Scene
} // Graphics
} // lug
