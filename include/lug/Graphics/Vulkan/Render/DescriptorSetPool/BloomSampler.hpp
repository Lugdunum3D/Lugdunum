#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Sampler.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

class LUG_GRAPHICS_API BloomSampler : public DescriptorSetPool<10> {
public:
    BloomSampler(Renderer& renderer);

    BloomSampler(const BloomSampler&) = delete;
    BloomSampler(BloomSampler&&) = delete;

    BloomSampler& operator=(const BloomSampler&) = delete;
    BloomSampler& operator=(BloomSampler&&) = delete;

    ~BloomSampler() = default;

    const DescriptorSet* allocate(
        const API::GraphicsPipeline& pipeline,
        const ::lug::Graphics::Vulkan::API::Image& image,
        const ::lug::Graphics::Vulkan::API::ImageView& imageView,
        const ::lug::Graphics::Vulkan::API::Sampler& sampler
    );
};

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
