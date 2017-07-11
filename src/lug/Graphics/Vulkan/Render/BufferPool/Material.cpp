#include <lug/Graphics/Vulkan/Render/BufferPool/Material.hpp>

#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace BufferPool {

Material::Material(Renderer& renderer) : BufferPool(renderer, {
    renderer.getDevice().getQueue("queue_graphics")->getQueueFamily()->getIdx(),
    renderer.getDevice().getQueue("queue_transfer")->getQueueFamily()->getIdx()
}) {}

const SubBuffer* Material::allocate(const API::CommandBuffer& cmdBuffer, ::lug::Graphics::Render::Material& material) {
    const auto& result = BufferPool::allocate(material.getHandle().value, material.isDirty());
    material.clearDirty();

    if (std::get<0>(result) && std::get<1>(result)) {
        // Update the buffer if the BufferPool told us that we need to
        cmdBuffer.updateBuffer(
            *std::get<1>(result)->getBuffer(),
            &material.getConstants(), sizeof(::
                lug::Graphics::Render::Material::Constants),
            std::get<1>(result)->getOffset()
        );
    }

    return std::get<1>(result);
}

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
