#include <lug/Graphics/Scene.hpp>

namespace lug {
namespace Graphics {

Scene::Scene() : _root{std::make_unique<SceneNode>("root")} {}

std::unique_ptr<SceneNode> Scene::createSceneNode(const std::string& name) {
    return std::make_unique<SceneNode>(name);
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
