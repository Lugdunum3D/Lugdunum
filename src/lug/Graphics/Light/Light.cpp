#include <lug/Graphics/Light/Light.hpp>

namespace lug {
namespace Graphics {
namespace Light {

Light::Light(const std::string& name, Type type) : MovableObject(name, MovableObject::Type::Light), _type(type) {}

void Light::needUpdate() {
    _needUpdate = true;
    _dirty = true;
}

} // Light
} // Graphics
} // lug
