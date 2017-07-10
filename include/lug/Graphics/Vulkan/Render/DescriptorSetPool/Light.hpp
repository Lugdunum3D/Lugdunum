#pragma once

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

class LUG_GRAPHICS_API Light : public DescriptorSetPool<10> {
public:
    Light(Renderer& renderer);

    Light(const Light&) = delete;
    Light(Light&&) = delete;

    Light& operator=(const Light&) = delete;
    Light& operator=(Light&&) = delete;

    ~Light() = default;

    const DescriptorSet* allocate(const BufferPool::SubBuffer& subBuffer);
};

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
