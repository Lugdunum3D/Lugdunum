#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/Material.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

Material::Material(Renderer& renderer) : DescriptorSetPool(renderer) {}

const DescriptorSet* Material::allocate(const BufferPool::SubBuffer& subBuffer) {
    const auto& result = DescriptorSetPool::allocate(
        reinterpret_cast<size_t>(static_cast<VkBuffer>(*subBuffer.getBuffer())),
        _renderer.getPipeline(Pipeline::getBaseId())->getPipelineAPI().getLayout()->getDescriptorSetLayouts()[2]
    );

    if (std::get<0>(result) && std::get<1>(result)) {
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
