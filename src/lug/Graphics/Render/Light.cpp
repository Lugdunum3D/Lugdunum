#include <lug/Graphics/Render/Light.hpp>

namespace lug {
namespace Graphics {
namespace Render {

Light::Light(const std::string& name, Type type) : Resource(Resource::Type::Light), _type(type), _name(name) {}

void Light::getData(Light::Data& lightData) {
    lightData.color = _color;

    if (_type == Type::Directional || _type == Type::Spot) {
        lightData.direction = _direction;
    }

    if (_type == Type::Point || _type == Type::Spot) {
        lightData.constantAttenuation = _constantAttenuation;
        lightData.distance = _distance;
        lightData.linearAttenuation = _linearAttenuation;
        lightData.quadraticAttenuation = _quadraticAttenuation;
    }

    if (_type == Type::Spot) {
        lightData.falloffAngle = _falloffAngle;
        lightData.falloffExponent = _falloffExponent;
    }
}

} // Render
} // Graphics
} // lug
