#include <lug/Graphics/Light/Spot.hpp>

namespace lug {
namespace Graphics {
namespace Light {

Spot::Spot(const std::string& name): Light(name, Light::Type::Spot) {}

void Spot::setPosition(const Math::Vec3f& position) {
    _position = position;
    needUpdate();
}

void Spot::setDirection(const Math::Vec3f& direction) {
    _direction = direction;
    needUpdate();
}

void Spot::setAngle(float angle) {
    _angle = angle;
    needUpdate();
}

void Spot::setOuterAngle(float outerAngle) {
    _outerAngle = outerAngle;
    needUpdate();
}

void* Spot::getData(uint32_t& size) {
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

    size = sizeof(Spot::LightData);
    return &_data;
}

} // Light
} // Graphics
} // lug
