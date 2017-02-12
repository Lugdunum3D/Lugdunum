#pragma once

#include <string>
#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

class SceneNode;

class LUG_GRAPHICS_API MovableObject {
public:
    enum class Type: uint8_t {
        MESH = 0,
        CAMERA = 1,
        LIGHT = 2
    };

public:
    MovableObject(const std::string& name, Type type);

    MovableObject(const MovableObject&) = delete;
    MovableObject(MovableObject&&) = delete;

    MovableObject& operator=(const MovableObject&) = delete;
    MovableObject& operator=(MovableObject&&) = delete;

    virtual ~MovableObject() = default;

    virtual void setParent(SceneNode* parent);
    SceneNode* getParent();
    const SceneNode* getParent() const;

    const std::string& getName() const;
    Type getType() const;

    virtual void needUpdate() = 0;

    bool isDirty() const;
    void isDirty(bool dirty);

protected:
    SceneNode* _parent{nullptr};
    std::string _name;
    Type _type;
    bool _needUpdate{true};

    // Flag to know if the node is dirty since the last frame
    bool _dirty{true};
};

#include <lug/Graphics/MovableObject.inl>

} // Graphics
} // lug
