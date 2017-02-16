#include <lug/Graphics/Light.hpp>

namespace lug {
namespace Graphics {

Light::Light(const std::string& name, Type type): MovableObject(name, MovableObject::Type::LIGHT), _type(type) {}

void Light::needUpdate() {
    _needUpdate = true;
    _dirty = true;
}

} // Graphics
} // lug
