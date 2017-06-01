#include <lug/Graphics/Vulkan/API/Builder/Sampler.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Sampler::Sampler(const API::Device& device) : _device{device} {}

bool Sampler::build(API::Sampler& sampler, VkResult* returnResult) {
    // Create the sampler creation information for vkCreateSampler
    const VkSamplerCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.magFilter */ _magFilter,
        /* createInfo.minFilter */ _minFilter,
        /* createInfo.mipmapMode */ _mipmapMode,
        /* createInfo.addressModeU */ _addressModeU,
        /* createInfo.addressModeV */ _addressModeV,
        /* createInfo.addressModeW */ _addressModeW,
        /* createInfo.mipLodBias */ _mipLodBias,
        /* createInfo.anisotropyEnable */ _anisotropyEnable,
        /* createInfo.maxAnisotropy */ _maxAnisotropy,
        /* createInfo.compareEnable */ _compareEnable,
        /* createInfo.compareOp */ _compareOp,
        /* createInfo.minLod */ _minLod,
        /* createInfo.maxLod */ _maxLod,
        /* createInfo.borderColor */ _borderColor,
        /* createInfo.unnormalizedCoordinates */ _unnormalizedCoordinates
    };

    // Create the sampler
    VkSampler vkSampler{VK_NULL_HANDLE};
    VkResult result = vkCreateSampler(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkSampler);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    sampler = API::Sampler(vkSampler, &_device);

    return true;
}

std::unique_ptr<API::Sampler> Sampler::build(VkResult* returnResult) {
    std::unique_ptr<API::Sampler> sampler = std::make_unique<API::Sampler>();
    return build(*sampler, returnResult) ? std::move(sampler) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
