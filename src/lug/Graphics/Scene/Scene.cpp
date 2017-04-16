#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/Graphics/Light/Directional.hpp>
#include <lug/Graphics/Light/Point.hpp>
#include <lug/Graphics/Light/Spot.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

Scene::Scene() : _root{std::make_unique<Node>(*this, "root")} {}

std::unique_ptr<Node> Scene::createSceneNode(const std::string& name, std::unique_ptr<MovableObject> object) {
    std::unique_ptr<Node> node = std::make_unique<Node>(*this, name);

    if (object) {
        node->attachMovableObject(std::move(object));
    }

    return node;
}

std::unique_ptr<MeshInstance> Scene::createMeshInstance(const std::string& name, Render::Mesh* mesh) {
    return std::make_unique<MeshInstance>(name, mesh);
}

std::unique_ptr<ModelInstance> Scene::createModelInstance(const std::string& name, Render::Model* model) {
    return std::make_unique<ModelInstance>(name, model);
}

std::unique_ptr<MovableCamera> Scene::createMovableCamera(const std::string& name, Render::Camera* camera) {
    return std::make_unique<MovableCamera>(name, camera);
}

std::unique_ptr<Light::Light> Scene::createLight(const std::string& name, Light::Light::Type type) {
    switch (type) {
        case Light::Light::Type::Directional:
            return std::make_unique<Light::Directional>(name);
        case Light::Light::Type::Point:
            return std::make_unique<Light::Point>(name);
        case Light::Light::Type::Spot:
            return std::make_unique<Light::Spot>(name);
    }

    return nullptr;
}

Node* Scene::getSceneNode(const std::string& name) {
    return _root->getNode(name);
}

const Node* Scene::getSceneNode(const std::string& name) const {
    return _root->getNode(name);
}

void Scene::fetchVisibleObjects(const Render::View* renderView, const Render::Camera* camera, Render::Queue& renderQueue) const {
    _root->fetchVisibleObjects(renderView, camera, renderQueue);
}

} // Scene
} // Graphics
} // lug
