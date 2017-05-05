#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Material.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace Builder {
class Material;
} // Builder

namespace Render {

/**
 * @brief     Class for Material
 */
class LUG_GRAPHICS_API Material final : public ::lug::Graphics::Render::Material {
    friend class ::lug::Graphics::Vulkan::Builder::Material;

public:
    Pipeline::Handle::MaterialPart _pipelineIdMaterialPart;

public:
    Material(const Material&) = delete;
    Material(Material&&) = delete;

    Material& operator=(const Material&) = delete;
    Material& operator=(Material&&) = delete;

    ~Material();

    void destroy();

private:
    /**
     * @brief      Constructs a Material
     *
     * @param[in]  name  The name of the Material
     * @param[in]  type  The type of the Material
     */
    Material(const std::string& name);

private:
    API::DeviceMemory _deviceMemory;
    API::Buffer _buffer;
};

} // Render
} // Vulkan
} // Graphics
} // lug
