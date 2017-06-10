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

    float getXMag() const;
    void setXMag(float xmag);

    float getYMag() const;
    void setYMag(float ymag);

protected:
    explicit Orthographic(const std::string& name);

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
