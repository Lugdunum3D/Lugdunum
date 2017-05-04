#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace Builder {
class Mesh;
} // Builder

namespace Render {

class LUG_GRAPHICS_API Mesh : public ::lug::Graphics::Render::Mesh {
    friend class ::lug::Graphics::Vulkan::Builder::Mesh;

public:
    struct PrimitiveSetData {
        Pipeline::Handle::PrimitivePart pipelineIdPrimitivePart;
        std::vector<std::unique_ptr<API::Buffer>> buffers;
    };

public:
    Mesh(const Mesh&) = delete;
    Mesh(Mesh&& mesh) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& mesh) = delete;

    ~Mesh() override final;

    void destroy();

private:
    explicit Mesh(const std::string& name);

private:
    std::unique_ptr<API::DeviceMemory> _deviceMemory{nullptr};
};

#include <lug/Graphics/Vulkan/Render/Mesh.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
