#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light.hpp>
#include <lug/Graphics/MeshInstance.hpp>
#include <lug/Graphics/MovableCamera.hpp>
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
    std::unique_ptr<MeshInstance> createMeshInstance(const std::string& name, Mesh* mesh = nullptr);
    std::unique_ptr<MovableCamera> createMovableCamera(const std::string& name, Camera* camera = nullptr);
    std::unique_ptr<Light> createLight(const std::string& name, Light::Type type);

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
