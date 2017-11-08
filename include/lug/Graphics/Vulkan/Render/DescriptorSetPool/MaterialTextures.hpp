#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

class LUG_GRAPHICS_API MaterialTextures : public DescriptorSetPool<10> {
public:
    MaterialTextures(Renderer& renderer);

    MaterialTextures(const MaterialTextures&) = delete;
    MaterialTextures(MaterialTextures&&) = delete;

    MaterialTextures& operator=(const MaterialTextures&) = delete;
    MaterialTextures& operator=(MaterialTextures&&) = delete;

    ~MaterialTextures() = default;

    const DescriptorSet* allocate(const API::GraphicsPipeline& pipeline, const std::vector<const ::lug::Graphics::Vulkan::Render::Texture*>& textures);
};

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
