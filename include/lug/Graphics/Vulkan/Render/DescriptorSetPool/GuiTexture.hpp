#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

class LUG_GRAPHICS_API GuiTexture : public DescriptorSetPool<10> {
public:
    GuiTexture(Renderer& renderer);

    GuiTexture(const GuiTexture&) = delete;
    GuiTexture(GuiTexture&&) = delete;

    GuiTexture& operator=(const GuiTexture&) = delete;
    GuiTexture& operator=(GuiTexture&&) = delete;

    ~GuiTexture() = default;

    const DescriptorSet* allocate(const API::GraphicsPipeline& pipeline, const ::lug::Graphics::Vulkan::Render::Texture* texture);
};

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
