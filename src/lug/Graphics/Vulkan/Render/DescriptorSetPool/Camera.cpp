#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/Camera.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

Camera::Camera(Renderer& renderer) : DescriptorSetPool(renderer) {}

const DescriptorSet* Camera::allocate(const BufferPool::SubBuffer& subBuffer) {
    const auto& result = DescriptorSetPool::allocate(
        reinterpret_cast<size_t>(static_cast<VkBuffer>(*subBuffer.getBuffer())),
        _renderer.getPipeline(Pipeline::getBaseId())->getPipelineAPI().getLayout()->getDescriptorSetLayouts()[0]
    );

    if (std::get<0>(result)) {
        std::get<1>(result)->getDescriptorSet().updateBuffers(
            0,
            0,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
            {
                {
                    static_cast<VkBuffer>(*subBuffer.getBuffer()),
                    0,
                    subBuffer.getSize()
                }
            }
        );
    }

    return std::get<1>(result);
}

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
