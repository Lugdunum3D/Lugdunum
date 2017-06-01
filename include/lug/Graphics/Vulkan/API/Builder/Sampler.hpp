#pragma once

#include <memory>

#include <lug/Graphics/Vulkan/API/Sampler.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class Sampler {
public:
    Sampler(const API::Device& device);

    Sampler(const Sampler&&) = delete;
    Sampler(Sampler&&) = delete;

    Sampler& operator=(const Sampler&&) = delete;
    Sampler& operator=(Sampler&&) = delete;

    ~Sampler() = default;

    // Setters
    void setMagFilter(VkFilter magFilter);
    void setMinFilter(VkFilter minFilter);
    void setMipmapMode(VkSamplerMipmapMode mipmapMode);
    void setAddressModeU(VkSamplerAddressMode addressModeU);
    void setAddressModeV(VkSamplerAddressMode addressModeV);
    void setAddressModeW(VkSamplerAddressMode addressModeW);
    void setMipLodBias(float mipLodBias);
    void setAnisotropyEnable(VkBool32 anisotropyEnable);
    void setMaxAnisotropy(float maxAnisotropy);
    void setCompareOp(VkCompareOp compareOp);
    void setMinLod(float minLod);
    void setMaxLod(float maxLod);
    void setBorderColor(VkBorderColor borderColor);
    void setUnnormalizedCoordinates(VkBool32 unnormalizedCoordinates);

    // Build methods
    bool build(API::Sampler& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Sampler> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    VkFilter _magFilter{VK_FILTER_LINEAR};
    VkFilter _minFilter{VK_FILTER_LINEAR};
    VkSamplerMipmapMode _mipmapMode{VK_SAMPLER_MIPMAP_MODE_LINEAR};
    VkSamplerAddressMode _addressModeU{VK_SAMPLER_ADDRESS_MODE_REPEAT};
    VkSamplerAddressMode _addressModeV{VK_SAMPLER_ADDRESS_MODE_REPEAT};
    VkSamplerAddressMode _addressModeW{VK_SAMPLER_ADDRESS_MODE_REPEAT};
    float _mipLodBias{0.0f};
    VkBool32 _anisotropyEnable{VK_FALSE};
    float _maxAnisotropy{0.0f};
    VkBool32 _compareEnable{VK_FALSE};
    VkCompareOp _compareOp{VK_COMPARE_OP_NEVER};
    float _minLod{0.0f};
    float _maxLod{1.0f};
    VkBorderColor _borderColor{VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE};
    VkBool32 _unnormalizedCoordinates{VK_FALSE};
};

#include <lug/Graphics/Vulkan/API/Builder/Sampler.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
