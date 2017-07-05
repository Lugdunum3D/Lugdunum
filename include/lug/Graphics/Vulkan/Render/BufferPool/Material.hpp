#pragma once

#include <lug/Graphics/Render/Material.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool/BufferPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class CommandBuffer;
} // API

namespace Render {
namespace BufferPool {

class LUG_GRAPHICS_API Material : public BufferPool<20, sizeof(::lug::Graphics::Render::Material::Constants) * 2> {
public:
    Material(Renderer& renderer);

    Material(const Material&) = delete;
    Material(Material&&) = delete;

    Material& operator=(const Material&) = delete;
    Material& operator=(Material&&) = delete;

    ~Material() = default;

    const SubBuffer* allocate(const API::CommandBuffer& cmdBuffer, ::lug::Graphics::Render::Material& material);
};

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
