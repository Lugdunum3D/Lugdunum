#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/GuiTexture.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

GuiTexture::GuiTexture(Renderer& renderer) : DescriptorSetPool(renderer) {}

const DescriptorSet* GuiTexture::allocate(const API::GraphicsPipeline& pipeline, const ::lug::Graphics::Vulkan::Render::Texture* texture) {
    const auto& result = DescriptorSetPool::allocate(
        reinterpret_cast<size_t>(static_cast<VkImage>(texture->getImage())),
        pipeline.getLayout()->getDescriptorSetLayouts()[0]
    );

    if (std::get<0>(result)) {
        std::get<1>(result)->getDescriptorSet().updateImages(
            0,
            0,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            {
                {
                    /* sampler       */ static_cast<VkSampler>(texture->getSampler()),
                    /* imageView     */ static_cast<VkImageView>(texture->getImageView()),
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
