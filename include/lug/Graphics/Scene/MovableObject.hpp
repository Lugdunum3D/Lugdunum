#pragma once

#include <string>
#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

class Node;

class LUG_GRAPHICS_API MovableObject {
public:
    enum class Type: uint8_t {
        Model = 0,
        Mesh = 1,
        Camera = 2,
        Light = 3
    };

public:
    MovableObject(const std::string& name, Type type);

    MovableObject(const MovableObject&) = delete;
    MovableObject(MovableObject&&) = delete;

    MovableObject& operator=(const MovableObject&) = delete;
    MovableObject& operator=(MovableObject&&) = delete;

    virtual ~MovableObject() = default;

    virtual void setParent(Node* parent);
    Node* getParent();
    const Node* getParent() const;

    const std::string& getName() const;
    Type getType() const;

    virtual void needUpdate() = 0;

    bool isDirty() const;
    void isDirty(bool dirty);

protected:
    Node* _parent{nullptr};
    std::string _name;
    Type _type;
    bool _needUpdate{true};

    // Flag to know if the node is dirty since the last frame
    bool _dirty{true};
};

#include <lug/Graphics/Scene/MovableObject.inl>

} // Scene
} // Graphics
} // lug
