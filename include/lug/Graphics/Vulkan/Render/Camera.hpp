#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Camera.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

class LUG_GRAPHICS_API Camera final : public lug::Graphics::Render::Camera {
public:
    explicit Camera(const std::string& name);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = default;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = default;

    ~Camera() = default;

    void update(const ::lug::Graphics::Render::View* renderView) override final;
};

} // Render
} // Vulkan
} // Graphics
} // lug
