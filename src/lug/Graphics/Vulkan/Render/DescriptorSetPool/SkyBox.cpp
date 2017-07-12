#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/SkyBox.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/Render/SkyBox.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

SkyBox::SkyBox(Renderer& renderer) : DescriptorSetPool(renderer) {}

const DescriptorSet* SkyBox::allocate(const ::lug::Graphics::Vulkan::Render::Texture* skyBox) {
    const auto& result = DescriptorSetPool::allocate(
        reinterpret_cast<size_t>(static_cast<VkImage>(skyBox->getImage())),
        Render::SkyBox::getPipeline().getLayout()->getDescriptorSetLayouts()[1]
    );

    if (std::get<0>(result)) {
        std::get<1>(result)->getDescriptorSet().updateImages(
            0,
            0,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            {
                {
                    /* sampler       */ static_cast<VkSampler>(skyBox->getSampler()),
                    /* imageView     */ static_cast<VkImageView>(skyBox->getImageView()),
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
