#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Mesh.hpp>
#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <memory>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Mesh : public lug::Graphics::Mesh {
public:
    explicit Mesh(const std::string& name, const std::vector<uint32_t>& queueFamilyIndices, const Device* device);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&& mesh) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& mesh) = delete;

    ~Mesh() override final;

    // TODO: Add reload() function
    bool load() override final;

    void destroy();

    const Buffer* getVertexBuffer() const;
    const Buffer* getIndexBuffer() const;

private:
    std::unique_ptr<Buffer> _vertexBuffer;
    std::unique_ptr<Buffer> _indexBuffer;

    std::unique_ptr<DeviceMemory> _vertexDeviceMemory{nullptr};
    std::unique_ptr<DeviceMemory> _indexDeviceMemory{nullptr};

    // Queue family indices used by the vertex and index buffers
    std::vector<uint32_t> _queueFamilyIndices;

    const Device* _device{nullptr};
};

#include <lug/Graphics/Vulkan/Mesh.inl>

} // Vulkan
} // Graphics
} // lug
