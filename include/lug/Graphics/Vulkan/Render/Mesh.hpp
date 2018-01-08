#pragma once

#include <memory>
#include <set>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Builder/Mesh.hpp>
#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

class LUG_GRAPHICS_API Mesh : public ::lug::Graphics::Render::Mesh {
    friend Resource::SharedPtr<lug::Graphics::Render::Mesh> Builder::Mesh::build(const ::lug::Graphics::Builder::Mesh&);

public:
    struct PrimitiveSetData {
        Pipeline::Id::Model::PrimitivePart pipelineIdPrimitivePart;
        std::vector<API::Buffer> buffers;
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
    API::DeviceMemory _deviceMemory;
};

#include <lug/Graphics/Vulkan/Render/Mesh.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
