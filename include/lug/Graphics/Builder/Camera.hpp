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
    void setFovY(float fovy);
    void setAspectRatio(float aspectRatio);

    // Orthographic
    void setXMag(float xmag);
    void setYMag(float ymag);

    // Both
    void setZNear(float znear);
    void setZFar(float zfar);

    Resource::SharedPtr<Render::Camera::Camera> build();

protected:
    Renderer& _renderer;

    std::string _name;
    Type _type{Type::Perspective};

    // Perspective
    float _fovy{0.0f};
    float _aspectRatio{0.0f};

    // Orthographic
    float _xmag{0.0f};
    float _ymag{0.0f};

    // Both
    float _znear{0.0f};
    float _zfar{0.0f};
};

#include <lug/Graphics/Builder/Camera.inl>

} // Builder
} // Graphics
} // lug
