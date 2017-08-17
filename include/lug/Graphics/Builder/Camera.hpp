#pragma once

#include <string>

#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Render/Camera/Camera.hpp>
#include <lug/System/Debug.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API Camera {
public:
    enum class Type : uint8_t {
        Perspective,
        Orthographic
    };

public:
    explicit Camera(Renderer& renderer);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    ~Camera() = default;

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the mesh.
     */
    void setName(const std::string& name);
    void setType(Type type);

    // Perspective
    void setFovY(float fovY);
    void setAspectRatio(float aspectRatio);

    // Orthographic
    void setXMag(float xMag);
    void setYMag(float yMag);

    // Both
    void setZNear(float zNear);
    void setZFar(float zFar);

    Resource::SharedPtr<Render::Camera::Camera> build();

protected:
    Renderer& _renderer;

    std::string _name{};
    Type _type{Type::Perspective};

    // Perspective
    float _fovY{0.0f};
    float _aspectRatio{0.0f};

    // Orthographic
    float _xMag{0.0f};
    float _yMag{0.0f};

    // Both
    float _zNear{0.0f};
    float _zFar{0.0f};
};

#include <lug/Graphics/Builder/Camera.inl>

} // Builder
} // Graphics
} // lug
