#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/Graphics/Render/Queue.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

Node::Node(Scene& scene, const std::string& name) : ::lug::Graphics::Node(name), _scene(scene) {}

Node* Node::createSceneNode(const std::string& name) {
    return _scene.createSceneNode(name);
}

void Node::attachLight(Resource::SharedPtr<Render::Light> light) {
    _light = light;
}

void Node::attachMeshInstance(Resource::SharedPtr<Render::Mesh> mesh, Resource::SharedPtr<Render::Material> material) {
    _meshInstance.mesh = mesh;

    const auto& primitiveSets = mesh->getPrimitiveSets();
    if (material) {
        _meshInstance.materials.resize(primitiveSets.size(), material);
    } else {
        _meshInstance.materials.resize(primitiveSets.size());
        for (uint32_t i = 0; i < primitiveSets.size(); ++i) {
            _meshInstance.materials[i] = primitiveSets[i].material;
        }
    }
}

void Node::attachCamera(Resource::SharedPtr<Render::Camera::Camera> camera) {
    camera->setParent(this);
    _camera = std::move(camera);
}

void Node::fetchVisibleObjects(const Renderer& renderer, const Render::View& renderView, const Render::Camera::Camera& camera, Render::Queue& renderQueue) const {
    for (const auto& child : _children) {
        static_cast<const Node*>(child)->fetchVisibleObjects(renderer, renderView, camera, renderQueue);
    }

    if (_meshInstance.mesh) {
        renderQueue.addMeshInstance(*const_cast<Node*>(this), renderer);
    }

    // Check the distance with the light
    if (_light && (_light->getDistance() == 0.0f || _light->getDistance() >= fabs((Math::Vec3f(const_cast<Node*>(this)->getAbsolutePosition() - camera.getParent()->getAbsolutePosition())).length()))) {
        renderQueue.addLight(*const_cast<Node*>(this));
    }
}

void Node::needUpdate() {
    ::lug::Graphics::Node::needUpdate();
    ::lug::Graphics::Render::DirtyObject::setDirty();

    if (_camera) {
        _camera->needUpdateView();
    }
}

} // Scene
} // Graphics
} // lug
