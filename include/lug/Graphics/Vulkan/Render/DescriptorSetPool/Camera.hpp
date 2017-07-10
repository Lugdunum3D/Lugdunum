#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

class LUG_GRAPHICS_API Camera : public DescriptorSetPool<10> {
public:
    Camera(Renderer& renderer);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    ~Camera() = default;

    const DescriptorSet* allocate(const BufferPool::SubBuffer& subBuffer);
};

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
