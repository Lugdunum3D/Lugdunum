#include <lug/Graphics/Vulkan/Render/BufferPool/Light.hpp>

#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace BufferPool {

Light::Light(Renderer& renderer) : BufferPool(renderer, {
    renderer.getDevice().getQueueFamily(VK_QUEUE_GRAPHICS_BIT)->getIdx(),
    renderer.getDevice().getQueueFamily(VK_QUEUE_TRANSFER_BIT)->getIdx()
}) {}

const SubBuffer* Light::allocate(uint32_t currentFrame, const API::CommandBuffer& cmdBuffer, const std::vector<::lug::Graphics::Scene::Node*> nodes) {
    // Generate hash
    size_t hash = nodes.size() * 2;
    for (auto node : nodes) {
        hash ^= reinterpret_cast<size_t>(node) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= node->getLight()->getHandle().value + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    const auto& result = BufferPool::allocate(hash, std::any_of(nodes.cbegin(), nodes.cend(), [&currentFrame](const ::lug::Graphics::Scene::Node* node) {
        return node->getLight()->isDirty(currentFrame) || node->isDirty(currentFrame);
    }));

    for (auto node : nodes) {
        node->getLight()->clearDirty(currentFrame);
        node->clearDirty(currentFrame);
    }

    if (std::get<0>(result) && std::get<1>(result)) {
        // Update the buffer if the BufferPool told us that we need to
        for (uint32_t i = 0; i < nodes.size(); ++i) {
            const auto& light = nodes[i]->getLight();

            ::lug::Graphics::Render::Light::Data lightData;
            light->getData(lightData, *nodes[i]);

            cmdBuffer.updateBuffer(
                *std::get<1>(result)->getBuffer(),
                &lightData, sizeof(lightData),
                std::get<1>(result)->getOffset() + ::lug::Graphics::Render::Light::strideShader * i
            );
        }

        const uint32_t lightsNb = static_cast<uint32_t>(nodes.size());
        cmdBuffer.updateBuffer(
            *std::get<1>(result)->getBuffer(),
            &lightsNb, sizeof(uint32_t),
            std::get<1>(result)->getOffset() + ::lug::Graphics::Render::Light::strideShader * 50);
    }

    return std::get<1>(result);
}

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
