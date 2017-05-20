#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Mesh::Mesh(
    const std::string& name,
    const std::vector<uint32_t>& queueFamilyIndices,
    const API::Device* device) : ::lug::Graphics::Render::Mesh(name), _queueFamilyIndices(queueFamilyIndices), _device(device) {}

Mesh::~Mesh() {
    destroy();
}

bool Mesh::load() {
    if (_loaded) {
        LUG_LOG.warn("RendererVulkan", "Attempt to load a mesh that is already loaded");
        return true;
    }

    {
        _vertexBuffer = API::Buffer::create(_device, (uint32_t)_queueFamilyIndices.size(), _queueFamilyIndices.data(), (uint32_t)vertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        if (!_vertexBuffer) {
            return false;
        }

        auto& requirements = _vertexBuffer->getRequirements();
        uint32_t memoryTypeIndex = API::DeviceMemory::findMemoryType(_device, requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        _vertexDeviceMemory = API::DeviceMemory::allocate(_device, requirements.size, memoryTypeIndex);

        if (!_vertexDeviceMemory) {
            return false;
        }

        _vertexBuffer->bindMemory(_vertexDeviceMemory.get());
        _vertexBuffer->updateData(vertices.data(), (uint32_t)vertices.size() * sizeof(Vertex));
    }

    {
        _indexBuffer = API::Buffer::create(_device, (uint32_t)_queueFamilyIndices.size(), _queueFamilyIndices.data(), (uint32_t)indices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        if (!_indexBuffer) {
            return false;
        }

        auto& requirements = _indexBuffer->getRequirements();
        uint32_t memoryTypeIndex = API::DeviceMemory::findMemoryType(_device, requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        _indexDeviceMemory = API::DeviceMemory::allocate(_device, requirements.size, memoryTypeIndex);

        if (!_indexDeviceMemory) {
            return false;
        }

        _indexBuffer->bindMemory(_indexDeviceMemory.get());
        _indexBuffer->updateData(indices.data(), (uint32_t)indices.size() * sizeof(uint32_t));
    }

    _loaded = true;

    return true;
}

void Mesh::destroy() {
    if (_vertexBuffer) {
        _vertexBuffer->destroy();
    }

    if (_indexBuffer) {
        _indexBuffer->destroy();
    }
}

} // Render
} // Vulkan
} // Graphics
} // lug
