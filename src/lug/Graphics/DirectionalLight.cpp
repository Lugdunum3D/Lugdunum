#include <lug/Graphics/DirectionalLight.hpp>

namespace lug {
namespace Graphics {

DirectionalLight::DirectionalLight(const std::string& name): Light(name, Light::Type::DirectionalLight) {}

void DirectionalLight::setDirection(const Math::Vec3f& direction) {
    _direction = direction;
    needUpdate();
}

void* DirectionalLight::getData(uint32_t& size) {
    if (_needUpdate) {
        _data.ambient = _ambient;
        _data.diffuse = _diffuse;
        _data.specular = _specular;
        _data.direction = _direction;
    }

    size = sizeof(DirectionalLight::LightData);
    return &_data;
}

} // Graphics
} // lug
