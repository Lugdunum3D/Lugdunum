#pragma once

#include <string>
#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

class SceneNode;

class LUG_GRAPHICS_API MovableObject {
public:
    MovableObject(const std::string& name);

    MovableObject(const MovableObject&) = delete;
    MovableObject(MovableObject&&) = delete;

    MovableObject& operator=(const MovableObject&) = delete;
    MovableObject& operator=(MovableObject&&) = delete;

    virtual ~MovableObject() = default;

    virtual void setParent(SceneNode* parent);
    SceneNode* getParent();

    const std::string& getName() const;

    virtual void needUpdate() = 0;

protected:
    SceneNode* _parent{nullptr};
    std::string _name;
};

#include <lug/Graphics/MovableObject.inl>

} // Graphics
} // lug
