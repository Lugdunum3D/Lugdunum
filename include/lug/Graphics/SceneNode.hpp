#pragma once

#include <memory>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Graphics/Node.hpp>

namespace lug {
namespace Graphics {

class RenderView;
class Camera;
class RenderQueue;

class LUG_GRAPHICS_API SceneNode : public Node {
public:
    SceneNode(const std::string& name);

    SceneNode(const SceneNode&) = delete;
    SceneNode(SceneNode&&) = delete;

    SceneNode& operator=(const SceneNode&) = delete;
    SceneNode& operator=(SceneNode&&) = delete;

    virtual ~SceneNode() = default;

    SceneNode* getSceneNode(const std::string& name);
    const SceneNode* getSceneNode(const std::string& name) const;

    void attachMovableObject(std::unique_ptr<MovableObject> movableObject);

    void fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const;

    virtual void needUpdate() override;

private:
    std::vector<std::unique_ptr<MovableObject>> _movableObjects;
};

#include <lug/Graphics/SceneNode.inl>

} // Graphics
} // lug
