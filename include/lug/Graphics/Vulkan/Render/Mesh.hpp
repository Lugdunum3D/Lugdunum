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
    struct PrimitiveSetData {
        //API::Pipeline::Handle::PrimitivePart pipelineIdPrimitivePart;
        std::vector<API::Buffer> buffers;
    };

public:
    explicit Mesh(const std::string& name, const std::vector<uint32_t>& queueFamilyIndices, const API::Device* device);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&& mesh) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& mesh) = delete;

    ~Mesh() override final;

    void destroy();

private:
    API::DeviceMemory _deviceMemory;

    // Queue family indices used by the vertex and index buffers
    std::vector<uint32_t> _queueFamilyIndices;

    const API::Device* _device{nullptr};
};

#include <lug/Graphics/Vulkan/Render/Mesh.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
