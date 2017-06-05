#pragma once

#include <list>
#include <string>
#include <vector>

#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Math/Constant.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API Light {
public:
    explicit Light(Renderer& renderer);

    Light(const Light&) = delete;
    Light(Light&&) = delete;

    Light& operator=(const Light&) = delete;
    Light& operator=(Light&&) = delete;

    virtual ~Light() = default;

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the mesh.
     */
    void setName(const std::string& name);

    void setType(Render::Light::Type type);
    void setColor(const Math::Vec4f& color);
    void setDirection(const Math::Vec3f& direction);
    void setConstantAttenuation(float constantAttenuation);
    void setDistance(float distance);
    void setLinearAttenuation(float linearAttenuation);
    void setQuadraticAttenuation(float quadraticAttenuation);
    void setFalloffAngle(float falloffAngle);
    void setFalloffExponent(float falloffExponent);

    Resource::SharedPtr<Render::Light> build();

protected:
    Renderer& _renderer;

    std::string _name;

    Render::Light::Type _type{Render::Light::Type::Ambient};

    Math::Vec4f _color{0.0f, 0.0f, 0.0f, 1.0f};
    Math::Vec3f _direction{0.0f, 0.0f, 0.0f};
    float _constantAttenuation{0.0f};
    float _distance{0.0f};
    float _linearAttenuation{1.0f};
    float _quadraticAttenuation{1.0f};
    float _falloffAngle{Math::halfPi<float>()};
    float _falloffExponent{0.0f};
};

#include <lug/Graphics/Builder/Light.inl>

} // Builder
} // Graphics
} // lug
