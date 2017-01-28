#include <lug/Graphics/Scene.hpp>

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
