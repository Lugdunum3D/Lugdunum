#include <lug/Graphics/Vulkan/Render/BufferPool/Camera.hpp>

#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace BufferPool {

Camera::Camera(Renderer& renderer) : BufferPool(renderer, {
    renderer.getDevice().getQueueFamily(VK_QUEUE_GRAPHICS_BIT)->getIdx(),
    renderer.getDevice().getQueueFamily(VK_QUEUE_TRANSFER_BIT)->getIdx()
}) {}

const SubBuffer* Camera::allocate(const API::CommandBuffer& cmdBuffer, ::lug::Graphics::Render::Camera::Camera& camera) {
    const auto& result = BufferPool::allocate(camera.getHandle().value, camera.isDirty());
    camera.clearDirty();

    if (std::get<0>(result)) {
        // Update the buffer if the BufferPool told us that we need to
        const Math::Mat4x4f cameraData[] = {
            camera.getViewMatrix(),
            camera.getProjectionMatrix()
        };

        cmdBuffer.updateBuffer(*std::get<1>(result)->getBuffer(), cameraData, sizeof(cameraData), std::get<1>(result)->getOffset());
    }

    return std::get<1>(result);
}

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
