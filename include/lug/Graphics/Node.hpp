#pragma once

#include <memory>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Quaternion.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API Node {
public:
    enum class TransformSpace : uint8_t {
        Local,
        Parent,
        World
    };

public:
    Node(const std::string& name);

    Node(const Node&) = delete;
    Node(Node&&) = delete;

    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;

    virtual ~Node() = default;

    void setParent(Node *parent);
    Node* getParent() const;

    const std::string& getName() const;

    Node* getNode(const std::string& name);
    const Node* getNode(const std::string& name) const;

    const Math::Vec3f& getAbsolutePosition();
    const Math::Quatf& getAbsoluteRotation();
    const Math::Vec3f& getAbsoluteScale();

    const Math::Mat4x4f& getTransform();

    const std::vector<Node*>& getChildren() const;

    void attachChild(Node& child);

    void translate(const Math::Vec3f& direction, TransformSpace space = TransformSpace::Local);
    void rotate(float angle, const Math::Vec3f& axis, TransformSpace space = TransformSpace::Local);
    void rotate(const Math::Quatf& quat, TransformSpace space = TransformSpace::Local);
    void scale(const Math::Vec3f& scale);

    void setPosition(const Math::Vec3f& position, TransformSpace space = TransformSpace::Local);
    void setRotation(float angle, const Math::Vec3f& axis, TransformSpace space = TransformSpace::Local);
    void setRotation(const Math::Quatf& rotation, TransformSpace space = TransformSpace::Local);

    /**
     * @brief      Rotates the node according to the direction given in parameter
     *
     * @param[in]  spaceTargetDirection  The direction we want the local direction point to (will be normalized), in local space
     * @param[in]  localDirectionVector  The local direction vector
     * @param[in]  localUpVector         The local up vector
     * @param[in]  space                 The space, defaults to local
     */
    void setDirection(const Math::Vec3f& spaceTargetDirection, const Math::Vec3f& localDirectionVector, const Math::Vec3f& localUpVector, TransformSpace space = TransformSpace::Local);

    /**
     * @brief      Rotates the node in order to look at the target position
     *
     * @param[in]  targetPosition        The target position
     * @param[in]  localDirectionVector  The local direction vector
     * @param[in]  localUpVector         The local up vector
     * @param[in]  space                 The space, defaults to local
     */
    void lookAt(const Math::Vec3f& targetPosition, const Math::Vec3f& localDirectionVector, const Math::Vec3f& localUpVector, TransformSpace space = TransformSpace::Local);

    virtual void needUpdate();
private:
    void update();

protected:
    Node* _parent{nullptr};

    std::string _name;
    std::vector<Node*> _children;

private:
    Math::Vec3f _position{Math::Vec3f(0.0f)};
    Math::Quatf _rotation{Math::Quatf::identity()};
    Math::Vec3f _scale{Math::Vec3f(1.0f)};

    Math::Vec3f _absolutePosition{Math::Vec3f(0.0f)};
    Math::Quatf _absoluteRotation{Math::Quatf::identity()};
    Math::Vec3f _absoluteScale{Math::Vec3f(1.0f)};

    Math::Mat4x4f _transform{Math::Mat4x4f::identity()};

    bool _needUpdate{true};
};

#include <lug/Graphics/Node.inl>

} // Graphics
} // lug
