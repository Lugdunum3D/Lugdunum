#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

/**
 * @brief     Class for Light
 */
class LUG_GRAPHICS_API Light : public MovableObject {
public:
    enum class Type: uint8_t {
        DIRECTIONAL_LIGHT = 0,
        POINT_LIGHT = 1,
        SPOTLIGHT = 2
    };

    struct LightData {

    };

public:
    /**
     * @brief      Constructs a light
     *
     * @param[in]  name  The name of the Node
     * @param[in]  type  The type of the Light
     */
    Light(const std::string& name, Type type);

    Light(const Light&) = delete;
    Light(Light&&) = delete;

    Light& operator=(const Light&) = delete;
    Light& operator=(Light&&) = delete;

    virtual ~Light() = default;

    /**
     * @copybrief MovableObject#needUpdate
     */
    void needUpdate() override final;

    /**
     * @brief      Gets the light type.
     *
     * @return     The light type.
     */
    Type getLightType() const;

    void setAmbient(const Math::Vec3f& ambient);
    void setDiffuse(const Math::Vec3f& diffuse);
    void setSpecular(const Math::Vec3f& specular);

    virtual void* getData(uint32_t& size) = 0;

protected:
    Math::Vec3f _ambient{0.1f, 0.1f, 0.1f}; // We don't want the ambient color to be too dominant
    Math::Vec3f _diffuse{1.0f, 1.0f, 1.0f};
    Math::Vec3f _specular{1.0f, 1.0f, 1.0f};

private:
    Type _type;
};

#include <lug/Graphics/Light.inl>

} // Graphics
} // lug
