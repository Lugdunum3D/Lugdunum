#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light/Light.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Scene/MeshInstance.hpp>
#include <lug/Graphics/Scene/MovableCamera.hpp>
#include <lug/Graphics/Scene/Node.hpp>

namespace lug {
namespace Graphics {

namespace Render {
class Camera;
class Queue;
class View;
} // Render

namespace Scene {

class LUG_GRAPHICS_API Scene : public Resource {
public:
    Scene(const std::string& name);

    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = delete;

    ~Scene() = default;

    std::unique_ptr<Node> createSceneNode(const std::string& name, std::unique_ptr<MovableObject> object = nullptr);
    std::unique_ptr<MeshInstance> createMeshInstance(const std::string& name, Render::Mesh* mesh = nullptr);
    std::unique_ptr<MovableCamera> createMovableCamera(const std::string& name, Render::Camera* camera = nullptr);
    std::unique_ptr<Light::Light> createLight(const std::string& name, Light::Light::Type type);

    Node* getRoot();
    const Node* getRoot() const;

    Node* getSceneNode(const std::string& name);
    const Node* getSceneNode(const std::string& name) const;

    void fetchVisibleObjects(const Render::View* renderView, const Render::Camera* camera, Render::Queue& renderQueue) const;

private:
    std::unique_ptr<Node> _root{nullptr};
};

#include <lug/Graphics/Scene/Scene.inl>

} // Scene
} // Graphics
} // lug
