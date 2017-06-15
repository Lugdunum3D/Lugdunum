#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Renderer.hpp>

#include <memory>

namespace lug {
namespace Graphics {
namespace Builder {

Light::Light(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<Render::Light> Light::build() {
    // Constructor of Light is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Render::Light(_name, _type)};
    Render::Light* light = static_cast<Render::Light*>(resource.get());

    light->_name = _name;
    light->_type = _type;
    light->_color = _color;
    light->_direction = _direction;
    light->_constantAttenuation = _constantAttenuation;
    light->_distance = _distance;
    light->_linearAttenuation = _linearAttenuation;
    light->_quadraticAttenuation = _quadraticAttenuation;
    light->_falloffAngle = _falloffAngle;
    light->_falloffExponent = _falloffExponent;

    return _renderer.getResourceManager()->add<::lug::Graphics::Render::Light>(std::move(resource));
}

} // Builder
} // Graphics
} // lug
