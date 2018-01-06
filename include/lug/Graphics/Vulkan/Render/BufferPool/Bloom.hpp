#pragma once

#include <cstdint>
#include <lug/Graphics/Vulkan/Render/BufferPool/BufferPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class CommandBuffer;
} // API

namespace Render {
namespace BufferPool {

class LUG_GRAPHICS_API Bloom : public BufferPool<10, sizeof(float)> {
public:
    Bloom(Renderer& renderer);

    Bloom(const Bloom&) = delete;
    Bloom(Bloom&&) = delete;

    Bloom& operator=(const Bloom&) = delete;
    Bloom& operator=(Bloom&&) = delete;

    ~Bloom() = default;

    const SubBuffer* allocate(uint32_t currentFrame, const API::CommandBuffer& cmdBuffer, float blurThreshold, bool dirty);
};

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
