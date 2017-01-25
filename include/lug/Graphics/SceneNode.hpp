#pragma once

#include <memory>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Mesh.hpp>
#include <lug/Graphics/MovableObject.hpp>

namespace lug {
namespace Graphics {

class RenderView;
class Camera;
class RenderQueue;

class LUG_GRAPHICS_API SceneNode {
public:
    SceneNode(const std::string& name);

    SceneNode(const SceneNode&) = delete;
    SceneNode(SceneNode&&) = delete;

    SceneNode& operator=(const SceneNode&) = delete;
    SceneNode& operator=(SceneNode&&) = delete;

    ~SceneNode() = default;

    const std::string& getName() const;

    SceneNode* getSceneNode(const std::string& name);
    const SceneNode* getSceneNode(const std::string& name) const;

    void attachChild(std::unique_ptr<SceneNode> child);
    void attachMovableObject(std::unique_ptr<MovableObject> movableObject);

    void fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const;

private:
    SceneNode* parent{nullptr};

    std::string _name;
    std::vector<std::unique_ptr<SceneNode>> children;
    std::vector<std::unique_ptr<MovableObject>> _movableObjects;
};

#include <lug/Graphics/SceneNode.inl>

} // Graphics
} // lug
