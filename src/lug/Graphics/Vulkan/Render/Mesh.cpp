#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Mesh::Mesh(
    const std::string& name,
    const std::set<uint32_t>& queueFamilyIndices,
    const API::Device& device) : ::lug::Graphics::Render::Mesh(name), _queueFamilyIndices(queueFamilyIndices), _device(device) {}

Mesh::~Mesh() {
    destroy();
}

bool Mesh::load() {
    if (_loaded) {
        LUG_LOG.warn("RendererVulkan: Attempt to load a mesh that is already loaded");
        return true;
    }
    VkResult result{VK_SUCCESS};

    // Create vertex buffer
    {
        API::Builder::Buffer bufferBuilder(_device);
        bufferBuilder.setQueueFamilyIndices(_queueFamilyIndices);
        bufferBuilder.setSize((uint32_t)vertices.size() * sizeof(Vertex));
        bufferBuilder.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!bufferBuilder.build(_vertexBuffer, &result)) {
            LUG_LOG.error("Mesh::load: Can't create vertex buffer: {}", result);
            return false;
        }
    }

    // Create index buffer
    {
        API::Builder::Buffer bufferBuilder(_device);
        bufferBuilder.setQueueFamilyIndices(_queueFamilyIndices);
        bufferBuilder.setSize((uint32_t)indices.size() * sizeof(uint32_t));
        bufferBuilder.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        if (!bufferBuilder.build(_indexBuffer, &result)) {
            LUG_LOG.error("Mesh::load: Can't create index buffer: {}", result);
            return false;
        }
    }

    // Create device memory
    {
        API::Builder::DeviceMemory deviceMemoryBuilder(_device);
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        if (!deviceMemoryBuilder.addBuffer(_vertexBuffer) ||
            !deviceMemoryBuilder.addBuffer(_indexBuffer)) {
            LUG_LOG.error("Model::load: Can't add buffer to device memory");
            return false;
        }
        if (!deviceMemoryBuilder.build(_deviceMemory, &result)) {
            LUG_LOG.error("Model::load: Can't create a device memory: {}", result);
            return false;
        }
    }

    if (!_vertexBuffer.updateData(vertices.data(), (uint32_t)vertices.size() * sizeof(Vertex))) {
        LUG_LOG.error("Model::load: Can't update the vertex buffer");
        return false;
    }

    if (!_indexBuffer.updateData(indices.data(), (uint32_t)indices.size() * sizeof(uint32_t))) {
        LUG_LOG.error("Model::load: Can't update the index buffer");
        return false;
    }

    _loaded = true;

    return true;
}

void Mesh::destroy() {
    _vertexBuffer.destroy();
    _indexBuffer.destroy();
}

} // Render
} // Vulkan
} // Graphics
} // lug
