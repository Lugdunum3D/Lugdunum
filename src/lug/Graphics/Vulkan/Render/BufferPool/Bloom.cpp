#include <lug/Graphics/Vulkan/Render/BufferPool/Bloom.hpp>

#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace BufferPool {

Bloom::Bloom(Renderer& renderer) : BufferPool(renderer, {
    renderer.getDevice().getQueue("queue_graphics")->getQueueFamily()->getIdx(),
    renderer.getDevice().getQueue("queue_transfer")->getQueueFamily()->getIdx()
}) {}

const SubBuffer* Bloom::allocate(uint32_t currentFrame, const API::CommandBuffer& cmdBuffer, float blurThreshold, bool dirty) {
    const auto& result = BufferPool::allocate(currentFrame, dirty);

    if (std::get<0>(result) && std::get<1>(result)) {
        cmdBuffer.updateBuffer(*std::get<1>(result)->getBuffer(), &blurThreshold, sizeof(blurThreshold), std::get<1>(result)->getOffset());
    }

    return std::get<1>(result);
}

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
