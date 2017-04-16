#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light/Light.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {
namespace Light {

/**
 * @brief     Class for Spot
 */
class LUG_GRAPHICS_API Spot final : public Light {
public:
    struct LightData{
        Math::Vec3f ambient;
        float padding;
        Math::Vec3f diffuse;
        float padding2;
        Math::Vec3f specular;
        float angle;
        Math::Vec3f position;
        float outerAngle;
        Math::Vec3f direction;
    };

public:
    /**
     * @brief      Constructs a Spot
     *
     * @param[in]  name  The name of the Light
     * @param[in]  type  The type of the Spot
     */
    Spot(const std::string& name);

    Spot(const Spot&) = delete;
    Spot(Spot&&) = delete;

    Spot& operator=(const Spot&) = delete;
    Spot& operator=(Spot&&) = delete;

    ~Spot() override final = default;

    void setPosition(const Math::Vec3f& position);
    void setDirection(const Math::Vec3f& direction);
    void setAngle(float angle);
    void setOuterAngle(float outerAngle);

    void* getData(uint32_t& size) override final;

private:
    Math::Vec3f _position{0.0f, 0.0f, 0.0f};
    Math::Vec3f _direction{0.0f, 0.0f, 0.0f};
    float _angle{15.0f};
    float _outerAngle{18.0f};

    LightData _data;
};

} // Light
} // Graphics
} // lug
