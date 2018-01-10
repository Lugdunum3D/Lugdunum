#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/BloomSampler.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

BloomSampler::BloomSampler(Renderer& renderer) : DescriptorSetPool(renderer) {}

const DescriptorSet* BloomSampler::allocate(
    const API::GraphicsPipeline& pipeline,
    const ::lug::Graphics::Vulkan::API::Image& image,
    const ::lug::Graphics::Vulkan::API::ImageView& imageView,
    const ::lug::Graphics::Vulkan::API::Sampler& sampler
) {
    const auto& result = DescriptorSetPool::allocate(
        reinterpret_cast<size_t>(static_cast<VkImage>(image)),
        pipeline.getLayout()->getDescriptorSetLayouts()[0]
    );

    if (std::get<0>(result) && std::get<1>(result)) {
        std::get<1>(result)->getDescriptorSet().updateImages(
            0,
            0,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            {
                {
                    /* sampler       */ static_cast<VkSampler>(sampler),
                    /* imageView     */ static_cast<VkImageView>(imageView),
                    /* imageLayout   */ VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
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
