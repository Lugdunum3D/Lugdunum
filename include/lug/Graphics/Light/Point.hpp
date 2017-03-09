#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light/Light.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {
namespace Light {

/**
 * @brief     Class for Point
 */
class LUG_GRAPHICS_API Point final : public Light {
public:
    struct LightData{
        Math::Vec3f ambient;
        float constant;
        Math::Vec3f diffuse;
        float linear;
        Math::Vec3f specular;
        float quadric;
        Math::Vec3f position;
    };

public:
    /**
     * @brief      Constructs a Point
     *
     * @param[in]  name  The name of the Light
     * @param[in]  type  The type of the Point
     */
    Point(const std::string& name);

    Point(const Point&) = delete;
    Point(Point&&) = delete;

    Point& operator=(const Point&) = delete;
    Point& operator=(Point&&) = delete;

    ~Point() override final = default;

    void setPosition(const Math::Vec3f& position);
    void setConstant(float constant);
    void setLinear(float linear);
    void setQuadric(float quadric);

    void* getData(uint32_t& size) override final;

private:
    Math::Vec3f _position{0.0f, 0.0f, 0.0f};
    float _constant{1.0f};
    float _linear{0.09f};
    float _quadric{0.032f};

    LightData _data;
};

} // Light
} // Graphics
} // lug
