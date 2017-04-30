#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Material.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/BufferView.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

/**
 * @brief     Class for Material
 */
class LUG_GRAPHICS_API Material : public ::lug::Graphics::Render::Material {
public:
    Pipeline::Handle::MaterialPart _pipelineIdMaterialPart;

public:
    /**
     * @brief      Constructs a Material
     *
     * @param[in]  name  The name of the Material
     * @param[in]  type  The type of the Material
     */
    Material(const std::string& name);

    Material(const Material&) = delete;
    Material(Material&&) = delete;

    Material& operator=(const Material&) = delete;
    Material& operator=(Material&&) = delete;

    ~Material() = default;

private:
    API::DeviceMemory _deviceMemory;
    API::Buffer _buffer;
    API::BufferView _bufferView;
};

} // Render
} // Vulkan
} // Graphics
} // lug
