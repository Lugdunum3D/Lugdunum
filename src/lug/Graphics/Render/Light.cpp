#include <lug/Graphics/Render/Light.hpp>

#include <lug/Graphics/Scene/Node.hpp>

namespace lug {
namespace Graphics {
namespace Render {

Light::Light(const std::string& name, Type type) : Resource(Resource::Type::Light, name), _type(type) {}

void Light::getData(Light::Data& lightData, Scene::Node& node) {
    lightData.type = static_cast<uint32_t>(_type);
    lightData.color = _color;

    if (_type != Type::Ambient) {
        lightData.position = node.getAbsolutePosition();
    }

    if (_type == Type::Directional || _type == Type::Spot) {
        lightData.direction = node.getAbsoluteRotation().transform() * _direction;
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
