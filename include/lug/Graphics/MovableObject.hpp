#pragma once

#include <string>
#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API MovableObject {
public:
    MovableObject(const std::string& name);

    MovableObject(const MovableObject&) = delete;
    MovableObject(MovableObject&&) = delete;

    MovableObject& operator=(const MovableObject&) = delete;
    MovableObject& operator=(MovableObject&&) = delete;

    virtual ~MovableObject() = default;
};

} // Graphics
} // lug
