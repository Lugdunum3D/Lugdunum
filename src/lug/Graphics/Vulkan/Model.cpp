#include <cstring>
#include <lug/Graphics/Vulkan/Model.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Model::Model(const std::string& name,
            const std::vector<uint32_t>& queueFamilyIndices,
            const Device* device) : lug::Graphics::Model(name), _queueFamilyIndices(queueFamilyIndices), _device(device) {}

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

    // Create vertex buffer
   {
        _vertexBuffer = Buffer::create(_device, (uint32_t)_queueFamilyIndices.size(), _queueFamilyIndices.data(), verticesNb * sizeof(Mesh::Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        if (!_vertexBuffer)
            return false;

        auto& requirements = _vertexBuffer->getRequirements();
        uint32_t memoryTypeIndex = DeviceMemory::findMemoryType(_device, requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        _vertexDeviceMemory = DeviceMemory::allocate(_device, requirements.size, memoryTypeIndex);
        if (!_vertexDeviceMemory) {
            return false;
        }

        _vertexBuffer->bindMemory(_vertexDeviceMemory.get());
    }

    // Create index buffer
    {
        _indexBuffer = Buffer::create(_device, (uint32_t)_queueFamilyIndices.size(), _queueFamilyIndices.data(), indicesNb * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        if (!_indexBuffer)
            return false;

        auto& requirements = _indexBuffer->getRequirements();
        uint32_t memoryTypeIndex = DeviceMemory::findMemoryType(_device, requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        _indexDeviceMemory = DeviceMemory::allocate(_device, requirements.size, memoryTypeIndex);
        if (!_indexDeviceMemory) {
            return false;
        }

        _indexBuffer->bindMemory(_indexDeviceMemory.get());
    }

    // Upload vertex data
    {
        Mesh::Vertex* vertices = new Mesh::Vertex[verticesNb];
        uint32_t offset = 0;

        for (const auto& mesh: _meshs) {
            std::memcpy(vertices + offset, mesh->vertices.data(), mesh->vertices.size() * sizeof(Mesh::Vertex));
            offset += static_cast<uint32_t>(mesh->vertices.size());
        }

        _vertexBuffer->updateData(vertices, verticesNb * sizeof(Mesh::Vertex));
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

        _indexBuffer->updateData(indices, indicesNb * sizeof(uint32_t));
        delete[] indices;
    }

    _loaded = true;

    return true;
}

void Model::destroy() {
    _vertexBuffer->destroy();
    _indexBuffer->destroy();
}

} // Vulkan
} // Graphics
} // lug
