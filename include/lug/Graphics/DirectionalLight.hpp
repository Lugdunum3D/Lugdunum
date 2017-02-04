#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Light.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

/**
 * @brief     Class for DirectionalLight
 */
class LUG_GRAPHICS_API DirectionalLight final : public Light {
public:
    struct LightData{
        Math::Vec3f ambient;
        float padding;
        Math::Vec3f diffuse;
        float padding2;
        Math::Vec3f specular;
        float padding3;
        Math::Vec3f direction;
    };

public:
    /**
     * @brief      Constructs a DirectionalLight
     *
     * @param[in]  name  The name of the Light
     * @param[in]  type  The type of the DirectionalLight
     */
    DirectionalLight(const std::string& name);

    DirectionalLight(const DirectionalLight&) = delete;
    DirectionalLight(DirectionalLight&&) = delete;

    DirectionalLight& operator=(const DirectionalLight&) = delete;
    DirectionalLight& operator=(DirectionalLight&&) = delete;

    ~DirectionalLight() override final = default;

    void setDirection(const Math::Vec3f& direction);

    void* getData(uint32_t& size) override final;

private:
    Math::Vec3f _direction{0.0f, 0.0f, 0.0f};

    LightData _data;
};

} // Graphics
} // lug
