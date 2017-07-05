#pragma once

#include <memory>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Node.hpp>
#include <lug/Graphics/Render/Camera/Camera.hpp>
#include <lug/Graphics/Render/DirtyObject.hpp>
#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Render/Material.hpp>
#include <lug/Graphics/Render/Mesh.hpp>

namespace lug {
namespace Graphics {

namespace Render {

class Queue;
class View;

} // Render

namespace Scene {

class Scene;

class LUG_GRAPHICS_API Node : public ::lug::Graphics::Node, public Render::DirtyObject {
    friend class Scene;

public:
    struct MeshInstance {
        Resource::SharedPtr<Render::Mesh> mesh{nullptr};
        std::vector<Resource::SharedPtr<Render::Material>> materials;
    };

public:
    Node(Scene& scene, const std::string& name);

    Node(const Node&) = delete;
    Node(Node&&) = delete;

    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;

    virtual ~Node() = default;

    Node* getNode(const std::string& name);
    const Node* getNode(const std::string& name) const;
    Scene& getScene();
    const Scene& getScene() const;

    Node* createSceneNode(const std::string& name);

    void attachLight(Resource::SharedPtr<Render::Light> light);
    void attachMeshInstance(Resource::SharedPtr<Render::Mesh> mesh, Resource::SharedPtr<Render::Material> material = nullptr);
    void attachCamera(Resource::SharedPtr<Render::Camera::Camera> camera);

    Render::Light* getLight();
    const Render::Light* getLight() const;

    const MeshInstance* getMeshInstance() const;

    Render::Camera::Camera* getCamera();
    const Render::Camera::Camera* getCamera() const;

    void fetchVisibleObjects(const Render::View& renderView, const Render::Camera::Camera& camera, Render::Queue& renderQueue) const;

    virtual void needUpdate() override;

private:
    Scene &_scene;

    Resource::SharedPtr<Render::Light> _light{nullptr};
    MeshInstance _meshInstance;
    Resource::SharedPtr<Render::Camera::Camera> _camera{nullptr};
};

#include <lug/Graphics/Scene/Node.inl>

} // Scene
} // Graphics
} // lug
