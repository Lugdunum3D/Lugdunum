#pragma once

#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool/BufferPool.hpp>

namespace lug {
namespace Graphics {

namespace Scene {
class Node;
} // Scene

namespace Vulkan {

namespace API {
class CommandBuffer;
} // API

namespace Render {
namespace BufferPool {

class LUG_GRAPHICS_API Light : public BufferPool<10, ::lug::Graphics::Render::Light::strideShader * 50 + sizeof(uint32_t)> {
public:
    Light(Renderer& renderer);

    Light(const Light&) = delete;
    Light(Light&&) = delete;

    Light& operator=(const Light&) = delete;
    Light& operator=(Light&&) = delete;

    ~Light() = default;

    const SubBuffer* allocate(uint32_t currentFrame, const API::CommandBuffer& cmdBuffer, const std::vector<::lug::Graphics::Scene::Node*> nodes);
};

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
