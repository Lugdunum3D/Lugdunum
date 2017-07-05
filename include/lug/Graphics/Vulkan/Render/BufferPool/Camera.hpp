#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/BufferPool.hpp>
#include <lug/Math/Matrix.hpp>

namespace lug {
namespace Graphics {

namespace Render {
namespace Camera {
class Camera;
} // Camera
} // Render

namespace Vulkan {

namespace API {
class CommandBuffer;
} // API

namespace Render {
namespace BufferPool {

class LUG_GRAPHICS_API Camera : public BufferPool<10, sizeof(Math::Mat4x4f) * 2> {
public:
    Camera(Renderer& renderer);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    ~Camera() = default;

    const SubBuffer* allocate(const API::CommandBuffer& cmdBuffer, ::lug::Graphics::Render::Camera::Camera& camera);
};

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
