#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/DirtyObject.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Math/Constant.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

// For friend
namespace Builder {
class Light;
} // Builder

namespace Scene {
class Node;
} // Scene

namespace Render {

/**
 * @brief     Class for Light
 */
class LUG_GRAPHICS_API Light : public Resource, public DirtyObject {
    friend class ::lug::Graphics::Builder::Light;

public:
    enum class Type: uint8_t {
        Ambient = 0,
        Directional = 1,
        Point = 2,
        Spot = 3
    };

    // No default values other than 0.0f, required fields will be filled by the function getData
    struct Data {
        Math::Vec3f position{0.0f, 0.0f, 0.0f};
        float distance{0.0f};
        Math::Vec4f color{0.0f, 0.0f, 0.0f, 0.0f};
        Math::Vec3f direction{0.0f, 0.0f, 0.0f};
        float constantAttenuation{0.0f};
        float linearAttenuation{0.0f};
        float quadraticAttenuation{0.0f};
        float falloffAngle{0.0f};
        float falloffExponent{0.0f};
        uint32_t type;
    };

    static constexpr uint32_t strideShader{ 80 };

public:
    Light(const Light&) = delete;
    Light(Light&&) = delete;

    Light& operator=(const Light&) = delete;
    Light& operator=(Light&&) = delete;

    ~Light() = default;

    void setType(Type type);
    void setColor(const Math::Vec4f& color);
    void setDirection(const Math::Vec3f& direction);
    void setConstantAttenuation(float constantAttenuation);
    void setDistance(float distance);
    void setLinearAttenuation(float linearAttenuation);
    void setQuadraticAttenuation(float quadraticAttenuation);
    void setFalloffAngle(float falloffAngle);
    void setFalloffExponent(float falloffExponent);

    /**
     * @brief      Gets the light type.
     *
     * @return     The light type.
     */
    Type getType() const;

    const Math::Vec4f& getColor() const;
    const Math::Vec3f& getDirection() const;
    float getConstantAttenuation() const;
    float getDistance() const;
    float getLinearAttenuation() const;
    float getQuadraticAttenuation() const;
    float getFalloffAngle() const;
    float getFalloffExponent() const;

    void getData(Light::Data& lightData, Scene::Node& node);

private:
    /**
     * @brief      Constructs a light
     *
     * @param[in]  name  The name of the Node
     * @param[in]  type  The type of the Light
     */
    Light(const std::string& name, Type type);

protected:
    Type _type{Type::Ambient};

    Math::Vec4f _color{0.0f, 0.0f, 0.0f, 1.0f};
    Math::Vec3f _direction{0.0f, 0.0f, 0.0f};
    float _constantAttenuation{0.0f};
    float _distance{0.0f};
    float _linearAttenuation{1.0f};
    float _quadraticAttenuation{1.0f};
    float _falloffAngle{Math::halfPi<float>()};
    float _falloffExponent{0.0f};
};

#include <lug/Graphics/Render/Light.inl>

} // Render
} // Graphics
} // lug
