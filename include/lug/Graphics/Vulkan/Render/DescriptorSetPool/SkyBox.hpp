#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

class LUG_GRAPHICS_API SkyBox : public DescriptorSetPool<10> {
public:
    SkyBox(Renderer& renderer);

    SkyBox(const SkyBox&) = delete;
    SkyBox(SkyBox&&) = delete;

    SkyBox& operator=(const SkyBox&) = delete;
    SkyBox& operator=(SkyBox&&) = delete;

    ~SkyBox() = default;

    const DescriptorSet* allocate(const ::lug::Graphics::Vulkan::Render::Texture* skyBox);
};

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
