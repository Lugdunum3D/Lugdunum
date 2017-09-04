#pragma once

#include <cstdint>
#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/SkyBox.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/Builder/SkyBox.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Vulkan {
namespace Render {

/**
 * @brief     Class for SkyBox
 */
class LUG_GRAPHICS_API SkyBox final : public ::lug::Graphics::Render::SkyBox {
    friend Resource::SharedPtr<lug::Graphics::Render::SkyBox> Builder::SkyBox::build(const ::lug::Graphics::Builder::SkyBox&);

public:
    SkyBox(const SkyBox&) = delete;
    SkyBox(SkyBox&&) = delete;

    SkyBox& operator=(const SkyBox&) = delete;
    SkyBox& operator=(SkyBox&&) = delete;

    ~SkyBox();

    void destroy();

    Resource::SharedPtr<lug::Graphics::Render::SkyBox> createIrradianceMap(lug::Graphics::Renderer& renderer) const override final;
    static const API::GraphicsPipeline& getPipeline();
    static const lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh> getMesh();

private:
    /**
     * @brief      Constructs a SkyBox
     *
     * @param[in]  name  The name of the SkyBox
     * @param[in]  type  The type of the SkyBox
     */
    SkyBox(const std::string& name);

private:
    static API::GraphicsPipeline _pipeline;
    static API::GraphicsPipeline _irradianceMapPipeline;
    static lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh> _mesh;

    /**
     * @brief      Store the number of skybox created to know when
     *             to create/destroy the pipeline and mesh
     *
     */
    static uint32_t _skyBoxCount;
};

#include <lug/Graphics/Vulkan/Render/SkyBox.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
