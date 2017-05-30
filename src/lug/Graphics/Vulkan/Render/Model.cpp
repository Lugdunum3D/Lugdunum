#include <cstring>
#include <lug/Graphics/Vulkan/Render/Model.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Model::Model(
    const std::string& name,
    const std::set<uint32_t>& queueFamilyIndices,
    const API::Device& device) : ::lug::Graphics::Render::Model(name), _queueFamilyIndices(queueFamilyIndices), _device(device) {}

Model::~Model() {
    destroy();
}

bool Model::load() {
    if (_loaded) {
        LUG_LOG.warn("RendererVulkan: Attempt to load a Model that is already loaded");
        return true;
    }

    uint32_t verticesNb = getVerticesSize();
    uint32_t indicesNb = getIndicesSize();
    VkResult result{VK_SUCCESS};

    // Create vertex buffer
    {
        API::Builder::Buffer bufferBuilder(_device);
        bufferBuilder.setQueueFamilyIndices(_queueFamilyIndices);
        bufferBuilder.setSize(verticesNb * sizeof(Mesh::Vertex));
        bufferBuilder.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!bufferBuilder.build(_vertexBuffer, &result)) {
            LUG_LOG.error("Model::load: Can't create vertex buffer: {}", result);
            return false;
        }
    }

    // Create index buffer
    {
        API::Builder::Buffer bufferBuilder(_device);
        bufferBuilder.setQueueFamilyIndices(_queueFamilyIndices);
        bufferBuilder.setSize(indicesNb * sizeof(uint32_t));
        bufferBuilder.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        if (!bufferBuilder.build(_indexBuffer, &result)) {
            LUG_LOG.error("Model::load: Can't create index buffer: {}", result);
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

    // Upload vertex data
    {
        Mesh::Vertex* vertices = new Mesh::Vertex[verticesNb];
        uint32_t offset = 0;

        for (const auto& mesh: _meshs) {
            std::memcpy(vertices + offset, mesh->vertices.data(), mesh->vertices.size() * sizeof(Mesh::Vertex));
            offset += static_cast<uint32_t>(mesh->vertices.size());
        }

        if (!_vertexBuffer.updateData(vertices, verticesNb * sizeof(Mesh::Vertex))) {
            LUG_LOG.error("Model::load: Can't update the vertex buffer");
            return false;
        }

        delete[] vertices;
    }

    // Upload index data
    {
        uint32_t* indices = new uint32_t[indicesNb];
        uint32_t offset = 0;

        for (const auto& mesh: _meshs) {
            std::memcpy(indices + offset, mesh->indices.data(), mesh->indices.size() * sizeof(uint32_t));
            offset += static_cast<uint32_t>(mesh->indices.size());
        }

        if (!_indexBuffer.updateData(indices, indicesNb * sizeof(uint32_t))) {
            LUG_LOG.error("Model::load: Can't update the vertex buffer");
            return false;
        }

        delete[] indices;
    }

    _loaded = true;

    return true;
}

void Model::destroy() {
    _vertexBuffer.destroy();
    _indexBuffer.destroy();
}

} // Render
} // Vulkan
} // Graphics
} // lug
