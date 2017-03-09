#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

class LUG_GRAPHICS_API Mesh : public ::lug::Graphics::Render::Mesh {
public:
    explicit Mesh(const std::string& name, const std::vector<uint32_t>& queueFamilyIndices, const API::Device* device);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&& mesh) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& mesh) = delete;

    ~Mesh() override final;

    // TODO: Add reload() function
    bool load() override final;

    void destroy();

    const API::Buffer* getVertexBuffer() const;
    const API::Buffer* getIndexBuffer() const;

private:
    std::unique_ptr<API::Buffer> _vertexBuffer;
    std::unique_ptr<API::Buffer> _indexBuffer;

    std::unique_ptr<API::DeviceMemory> _vertexDeviceMemory{nullptr};
    std::unique_ptr<API::DeviceMemory> _indexDeviceMemory{nullptr};

    // Queue family indices used by the vertex and index buffers
    std::vector<uint32_t> _queueFamilyIndices;

    const API::Device* _device{nullptr};
};

#include <lug/Graphics/Vulkan/Render/Mesh.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
