#pragma once

#include <lug/Graphics/Render/Camera/Camera.hpp>

namespace lug {
namespace Graphics {

namespace Builder {
class Camera;
} // Builder

namespace Render {
namespace Camera {

class LUG_GRAPHICS_API Perspective : public Camera {
    friend class Builder::Camera;

public:
    Perspective(const Perspective&) = delete;
    Perspective(Perspective&&) = delete;

    Perspective& operator=(const Perspective&) = delete;
    Perspective& operator=(Perspective&&) = delete;

    ~Perspective() = default;

    /**
     * @brief      Gets the field of view
     *
     * @return     The field of view value
     */
    float getFovY() const;
    /**
     * @brief      Sets the field of view
     *
     * @param[in]  fovy    The field of view value
     */
    void setFovY(float fovy);

    /**
     * @brief      Gets the aspect ratio
     *
     * @return     The aspect ratio value
     */
    float getAspectRatio() const;
    /**
     * @brief      Sets the aspect ratio
     *
     * @param[in]  aspectRatio    The aspect ratio value
     */
    void setAspectRatio(float aspectRatio);

protected:
    explicit Perspective(const std::string& name);

    /**
     * @brief      Update the projection matrix
     */
    void updateProj() override final;

private:
    float _fovy{0.0f};
    float _aspectRatio{0.0f};
};

#include <lug/Graphics/Render/Camera/Perspective.inl>

} // Camera
} // Render
} // Graphics
} // lug
