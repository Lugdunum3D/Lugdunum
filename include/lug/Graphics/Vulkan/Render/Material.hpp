#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Material.hpp>
#include <lug/Graphics/Vulkan/Builder/Material.hpp>
#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

/**
 * @brief     Class for Material
 */
class LUG_GRAPHICS_API Material final : public ::lug::Graphics::Render::Material {
    friend Resource::SharedPtr<lug::Graphics::Render::Material> Builder::Material::build(const ::lug::Graphics::Builder::Material&);

public:
    Material(const Material&) = delete;
    Material(Material&&) = delete;

    Material& operator=(const Material&) = delete;
    Material& operator=(Material&&) = delete;

    ~Material();

    void destroy();

    Pipeline::Id::MaterialPart getPipelineId();

private:
    /**
     * @brief      Constructs a Material
     *
     * @param[in]  name  The name of the Material
     * @param[in]  type  The type of the Material
     */
    Material(const std::string& name);

private:
    Pipeline::Id::MaterialPart _pipelineIdMaterialPart;
};

} // Render
} // Vulkan
} // Graphics
} // lug
