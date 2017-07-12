#pragma once

#include <list>
#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Render/SkyBox.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Scene/Node.hpp>

namespace lug {
namespace Graphics {

namespace Builder {
class Scene;
} // Builder

namespace Render {
namespace Camera {
class Camera;
} // Camera

class Queue;
class View;
} // Render

namespace Scene {

class LUG_GRAPHICS_API Scene : public Resource {
    friend class Builder::Scene;

public:
    Scene() = default;

    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = delete;

    ~Scene() = default;

    Node* createSceneNode(const std::string& name);

    void setSkyBox(Resource::SharedPtr<Render::SkyBox> skyBox);

    Node& getRoot();
    const Node& getRoot() const;

    Node* getSceneNode(const std::string& name);
    const Node* getSceneNode(const std::string& name) const;
    const Resource::SharedPtr<Render::SkyBox> getSkyBox() const;

    void fetchVisibleObjects(const Render::View& renderView, const Render::Camera::Camera& camera, Render::Queue& renderQueue) const;

private:
    Scene(const std::string& name);

private:
    Node _root;

    Resource::SharedPtr<Render::SkyBox> _skyBox{nullptr};

    std::list<Node> _nodes;
};

#include <lug/Graphics/Scene/Scene.inl>

} // Scene
} // Graphics
} // lug
