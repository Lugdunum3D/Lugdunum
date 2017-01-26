#pragma once

#include <lug/Graphics/Camera.hpp>
#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Camera final : public lug::Graphics::Camera {
public:
    Camera(const std::string& name);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = default;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = default;

    ~Camera() = default;

    void update(const RenderView* renderView) override final;
};

} // Vulkan
} // Graphics
} // lug
