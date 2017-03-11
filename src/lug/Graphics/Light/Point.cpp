#include <lug/Graphics/Light/Point.hpp>

namespace lug {
namespace Graphics {
namespace Light {

Point::Point(const std::string& name): Light(name, Light::Type::Point) {}

void Point::setPosition(const Math::Vec3f& position) {
    _position = position;
    needUpdate();
}

void Point::setConstant(float constant) {
    _constant = constant;
    needUpdate();
}

void Point::setLinear(float linear) {
    _linear = linear;
    needUpdate();
}

void Point::setQuadric(float quadric) {
    _quadric = quadric;
    needUpdate();
}

void* Point::getData(uint32_t& size) {
    if (_needUpdate) {
        _data.ambient = _ambient;
        _data.diffuse = _diffuse;
        _data.specular = _specular;
        _data.position = _position;
        _data.constant = _constant;
        _data.linear = _linear;
        _data.quadric = _quadric;
        _needUpdate = false;
    }

    size = sizeof(Point::LightData);
    return &_data;
}

} // Light
} // Graphics
} // lug
