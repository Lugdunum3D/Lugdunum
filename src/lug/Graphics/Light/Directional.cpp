#include <lug/Graphics/Light/Directional.hpp>

namespace lug {
namespace Graphics {
namespace Light {

Directional::Directional(const std::string& name): Light(name, Light::Type::Directional) {}

void Directional::setDirection(const Math::Vec3f& direction) {
    _direction = direction;
    needUpdate();
}

void* Directional::getData(uint32_t& size) {
    if (_needUpdate) {
        _data.ambient = _ambient;
        _data.diffuse = _diffuse;
        _data.specular = _specular;
        _data.direction = _direction;
        _needUpdate = false;
    }

    size = sizeof(Directional::LightData);
    return &_data;
}

} // Light
} // Graphics
} // lug
