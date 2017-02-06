#include <lug/Graphics/Scene.hpp>
#include <lug/Graphics/DirectionalLight.hpp>
#include <lug/Graphics/PointLight.hpp>
#include <lug/Graphics/Spotlight.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {

Scene::Scene() : _root{std::make_unique<SceneNode>("root")} {}

std::unique_ptr<SceneNode> Scene::createSceneNode(const std::string& name) {
    return std::make_unique<SceneNode>(name);
}

std::unique_ptr<MeshInstance> Scene::createMeshInstance(const std::string& name, Mesh* mesh) {
    return std::make_unique<MeshInstance>(name, mesh);
}

std::unique_ptr<MovableCamera> Scene::createMovableCamera(const std::string& name, Camera* camera) {
    return std::make_unique<MovableCamera>(name, camera);
}

std::unique_ptr<Light> Scene::createLight(const std::string& name, Light::Type type) {
    switch (type) {
        case Light::Type::DirectionalLight:    return std::make_unique<DirectionalLight>(name);
        case Light::Type::PointLight:          return std::make_unique<PointLight>(name);
        case Light::Type::Spotlight:            return std::make_unique<Spotlight>(name);
        default:
            LUG_LOG.warn("Scene::createLight: Unknow light type");
            return nullptr;
    }
}

SceneNode* Scene::getSceneNode(const std::string& name) {
    return _root->getSceneNode(name);
}

const SceneNode* Scene::getSceneNode(const std::string& name) const {
    return _root->getSceneNode(name);
}

void Scene::fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const {
    _root->fetchVisibleObjects(renderView, camera, renderQueue);
}

} // Graphics
} // lug
