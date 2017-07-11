#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/MaterialTextures.hpp>

#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

MaterialTextures::MaterialTextures(Renderer& renderer) : DescriptorSetPool(renderer) {}

const DescriptorSet* MaterialTextures::allocate(const API::GraphicsPipeline& pipeline, const std::vector<const ::lug::Graphics::Vulkan::Render::Texture*> textures) {
    // Generate hash
    size_t hash = textures.size();
    for (uint32_t i = 0; i < textures.size(); ++i) {
        hash ^= textures[i]->getHandle().value + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    const auto& result = DescriptorSetPool::allocate(
        hash,
        pipeline.getLayout()->getDescriptorSetLayouts()[3]
    );

    if (std::get<0>(result)) {
        std::vector<VkDescriptorImageInfo> imageInfos(textures.size());

        for (uint32_t i = 0; i < textures.size(); ++i) {
            imageInfos[i].sampler = static_cast<VkSampler>(textures[i]->getSampler());
            imageInfos[i].imageView = static_cast<VkImageView>(textures[i]->getImageView());
            imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        std::get<1>(result)->getDescriptorSet().updateImages(
            0,
            0,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            std::move(imageInfos)
        );
    }

    return std::get<1>(result);
}

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
