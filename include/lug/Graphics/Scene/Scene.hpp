#pragma once

#include <list>
#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Resource.hpp>
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
    Scene() = default;

    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = delete;

    ~Scene() = default;

    Node* createSceneNode(const std::string& name);
    std::unique_ptr<MovableCamera> createMovableCamera(const std::string& name, Render::Camera* camera = nullptr);

    Node& getRoot();
    const Node& getRoot() const;

    Node* getSceneNode(const std::string& name);
    const Node* getSceneNode(const std::string& name) const;

    void fetchVisibleObjects(const Render::View* renderView, const Render::Camera* camera, Render::Queue& renderQueue) const;

private:
    Scene(const std::string& name);

private:
    Node _root;

    std::list<Node> _nodes;
};

#include <lug/Graphics/Scene/Scene.inl>

} // Scene
} // Graphics
} // lug
