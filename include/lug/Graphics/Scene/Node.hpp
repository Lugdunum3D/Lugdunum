#pragma once

#include <memory>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Node.hpp>
#include <lug/Graphics/Scene/MovableObject.hpp>

namespace lug {
namespace Graphics {

namespace Render {
class Camera;
class Queue;
class View;
} // Render

namespace Scene {

class LUG_GRAPHICS_API Node : public ::lug::Graphics::Node {
public:
    Node(const std::string& name);

    Node(const Node&) = delete;
    Node(Node&&) = delete;

    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;

    virtual ~Node() = default;

    Node* getNode(const std::string& name);
    const Node* getNode(const std::string& name) const;

    void attachMovableObject(std::unique_ptr<MovableObject> movableObject);

    void fetchVisibleObjects(const Render::View* renderView, const Render::Camera* camera, Render::Queue& renderQueue) const;

    virtual void needUpdate() override;

private:
    std::vector<std::unique_ptr<MovableObject>> _movableObjects;
};

#include <lug/Graphics/Scene/Node.inl>

} // Scene
} // Graphics
} // lug
