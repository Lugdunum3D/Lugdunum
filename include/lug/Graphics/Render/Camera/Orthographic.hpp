#pragma once

#include <lug/Graphics/Render/Camera/Camera.hpp>

namespace lug {
namespace Graphics {

namespace Builder {
class Camera;
} // Builder

namespace Render {
namespace Camera {

class LUG_GRAPHICS_API Orthographic : public Camera {
    friend class Builder::Camera;

public:
    Orthographic(const Orthographic&) = delete;
    Orthographic(Orthographic&&) = delete;

    Orthographic& operator=(const Orthographic&) = delete;
    Orthographic& operator=(Orthographic&&) = delete;

    ~Orthographic() = default;

    /**
     * @brief      Gets the projection width
     *
     * @return     The projection width
     */
    float getXMag() const;
    /**
     * @brief      Sets the projection width
     *
     * @param[in]  xmag    The projection width
     */
    void setXMag(float xmag);

    /**
     * @brief      Gets the projection height
     *
     * @return     The projection height
     */
    float getYMag() const;
    /**
     * @brief      Sets the projection height
     *
     * @param[in]  ymag    The projection height
     */
    void setYMag(float ymag);

protected:
    explicit Orthographic(const std::string& name);

    /**
     * @brief      Update the projection matrix
     */
    void updateProj() override final;

private:
    float _xmag{0.0f};
    float _ymag{0.0f};
};

#include <lug/Graphics/Render/Camera/Orthographic.inl>

} // Camera
} // Render
} // Graphics
} // lug
