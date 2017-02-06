#include <lug/Graphics/PointLight.hpp>

namespace lug {
namespace Graphics {

PointLight::PointLight(const std::string& name): Light(name, Light::Type::PointLight) {}

void PointLight::setPosition(const Math::Vec3f& position) {
    _position = position;
    needUpdate();
}

void PointLight::setConstant(float constant) {
    _constant = constant;
    needUpdate();
}

void PointLight::setLinear(float linear) {
    _linear = linear;
    needUpdate();
}

void PointLight::setQuadric(float quadric) {
    _quadric = quadric;
    needUpdate();
}

void* PointLight::getData(uint32_t& size) {
    if (_needUpdate) {
        _data.ambient = _ambient;
        _data.diffuse = _diffuse;
        _data.specular = _specular;
        _data.position = _position;
        _data.constant = _constant;
        _data.linear = _linear;
        _data.quadric = _quadric;
    }

    size = sizeof(PointLight::LightData);
    return &_data;
}

} // Graphics
} // lug
