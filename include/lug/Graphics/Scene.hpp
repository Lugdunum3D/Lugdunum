#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/SceneNode.hpp>

namespace lug {
namespace Graphics {

class RenderView;
class Camera;
class RenderQueue;

class LUG_GRAPHICS_API Scene {
public:
    Scene();

    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = delete;

    ~Scene() = default;

    std::unique_ptr<SceneNode> createSceneNode(const std::string& name);

    SceneNode* getRoot();
    const SceneNode* getRoot() const;

    SceneNode* getSceneNode(const std::string& name);
    const SceneNode* getSceneNode(const std::string& name) const;

    void fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const;

private:
    std::unique_ptr<SceneNode> _root{nullptr};
};

#include <lug/Graphics/Scene.inl>

} // Graphics
} // lug
