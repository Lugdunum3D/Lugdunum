#pragma once

#include <memory>
#include <set>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Model.hpp>
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

class LUG_GRAPHICS_API Model : public ::lug::Graphics::Render::Model {
public:
    explicit Model(const std::string& name, const std::set<uint32_t>& queueFamilyIndices, const API::Device& device);

    Model(const Model&) = delete;
    Model(Model&& Model) = delete;

    Model& operator=(const Model&) = delete;
    Model& operator=(Model&& Model) = delete;

    ~Model() override final;

    // TODO: Add reload() function
    bool load() override final;

    void destroy();

    const API::Buffer* getVertexBuffer() const;
    const API::Buffer* getIndexBuffer() const;

private:
    API::Buffer _vertexBuffer;
    API::Buffer _indexBuffer;

    API::DeviceMemory _deviceMemory;

    // Queue family indices used by the vertex and index buffers
    std::set<uint32_t> _queueFamilyIndices;

    const API::Device& _device;
};

#include <lug/Graphics/Vulkan/Render/Model.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
