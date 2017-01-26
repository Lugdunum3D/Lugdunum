#pragma once

#include <memory>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Mesh.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Quaternion.hpp>
#include <lug/Math/Vector.hpp>

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

    const Math::Vector<3, float>& getAbsolutePosition() const;
    const Math::Quaternion<float>& getAbsoluteRotation() const;
    const Math::Vector<3, float>& getAbsoluteScale() const;

    void attachChild(std::unique_ptr<SceneNode> child);
    void attachMovableObject(std::unique_ptr<MovableObject> movableObject);

    void fetchVisibleObjects(const RenderView* renderView, const Camera* camera, RenderQueue& renderQueue) const;

    void translate(const Math::Vector<3, float>& position);
    void rotate(const Math::Vector<3, float>& rotation, float amount);
    void scale(const Math::Vector<3, float>& scale);

    void update();

private:
    SceneNode* _parent{nullptr};

    std::string _name;
    std::vector<std::unique_ptr<SceneNode>> _children;
    std::vector<std::unique_ptr<MovableObject>> _movableObjects;

    Math::Vector<3, float> _position;
    Math::Quaternion<float> _rotation;
    Math::Vector<3, float> _scale;

    Math::Vector<3, float> _absolutePosition;
    Math::Quaternion<float> _absoluteRotation;
    Math::Vector<3, float> _absoluteScale;

    Math::Matrix<4, 4, float> _transform;

    bool _needUpdate{true};
};

#include <lug/Graphics/SceneNode.inl>

} // Graphics
} // lug
