#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/Graphics/Render/Queue.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

Node::Node(Scene& scene, const std::string& name) : ::lug::Graphics::Node(name), _scene(scene) {}

Node* Node::createSceneNode(const std::string& name) {
    Node* node = _scene.createSceneNode(name);

    attachChild(*node);
    return node;
}

void Node::attachLight(Resource::SharedPtr<Render::Light> light) {
    _light = light;
}

void Node::attachMeshInstance(Resource::SharedPtr<Render::Mesh> mesh, Resource::SharedPtr<Render::Material> material) {
    _meshInstance.mesh = mesh;

    const auto& primitiveSets = mesh->getPrimitiveSets();
    if (material) {
        _meshInstance.materials.resize(primitiveSets.size(), material);
    }
    else {
        _meshInstance.materials.resize(primitiveSets.size());
        for (uint32_t i = 0; i < primitiveSets.size(); ++i) {
            _meshInstance.materials[i] = primitiveSets[i].material;
        }
    }

}

void Node::fetchVisibleObjects(const Render::View* renderView, const Render::Camera* camera, Render::Queue& renderQueue) const {
    for (const auto& child : _children) {
        static_cast<const Node*>(child)->fetchVisibleObjects(renderView, camera, renderQueue);
    }

    renderQueue.addMeshInstance(*const_cast<Node*>(this), _meshInstance);
    renderQueue.addLight(*const_cast<Node*>(this), _light);
}

void Node::needUpdate() {
    ::lug::Graphics::Node::needUpdate();

    //for (auto& object : _movableObjects) {
        // object->needUpdate();
    //}
}

} // Scene
} // Graphics
} // lug
