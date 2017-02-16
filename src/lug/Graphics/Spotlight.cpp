#include <lug/Graphics/Spotlight.hpp>

namespace lug {
namespace Graphics {

Spotlight::Spotlight(const std::string& name): Light(name, Light::Type::Spotlight) {}

void Spotlight::setPosition(const Math::Vec3f& position) {
    _position = position;
    needUpdate();
}

void Spotlight::setDirection(const Math::Vec3f& direction) {
    _direction = direction;
    needUpdate();
}

void Spotlight::setAngle(float angle) {
    _angle = angle;
    needUpdate();
}

void Spotlight::setOuterAngle(float outerAngle) {
    _outerAngle = outerAngle;
    needUpdate();
}

void* Spotlight::getData(uint32_t& size) {
    if (_needUpdate) {
        _data.ambient = _ambient;
        _data.diffuse = _diffuse;
        _data.specular = _specular;
        _data.position = _position;
        _data.direction = _direction;
        _data.angle = _angle;
        _data.outerAngle = _outerAngle;
        _needUpdate = false;
    }

    size = sizeof(Spotlight::LightData);
    return &_data;
}

} // Graphics
} // lug
