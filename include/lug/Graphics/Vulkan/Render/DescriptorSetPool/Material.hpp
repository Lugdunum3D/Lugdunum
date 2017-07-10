#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

class LUG_GRAPHICS_API Material : public DescriptorSetPool<10> {
public:
    Material(Renderer& renderer);

    Material(const Material&) = delete;
    Material(Material&&) = delete;

    Material& operator=(const Material&) = delete;
    Material& operator=(Material&&) = delete;

    ~Material() = default;

    const DescriptorSet* allocate(const BufferPool::SubBuffer& subBuffer);
};

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
