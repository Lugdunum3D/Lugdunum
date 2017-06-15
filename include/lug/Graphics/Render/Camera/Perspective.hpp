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

    float getFovY() const;
    void setFovY(float fovy);

    float getAspectRatio() const;
    void setAspectRatio(float aspectRatio);

protected:
    explicit Perspective(const std::string& name);

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
