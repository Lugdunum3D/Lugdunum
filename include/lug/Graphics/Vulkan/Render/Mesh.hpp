#pragma once

#include <memory>
#include <set>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class Device;
} // API

namespace Render {

class LUG_GRAPHICS_API Mesh : public ::lug::Graphics::Render::Mesh {
public:
    explicit Mesh(const std::string& name, const std::set<uint32_t>& queueFamilyIndices, const API::Device& device);

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
    API::Buffer _vertexBuffer;
    API::Buffer _indexBuffer;

    // Queue family indices used by the vertex and index buffers
    std::set<uint32_t> _queueFamilyIndices;

    API::DeviceMemory _deviceMemory;

    const API::Device& _device;
};

#include <lug/Graphics/Vulkan/Render/Mesh.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
