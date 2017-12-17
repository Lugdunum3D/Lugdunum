#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Render/Texture.hpp>

namespace lug {
namespace Graphics {

class Renderer;

// For friend
namespace Builder {
class SkyBox;
} // Builder

namespace Render {

class LUG_GRAPHICS_API SkyBox : public Resource {
    friend class ::lug::Graphics::Builder::SkyBox;

public:
    SkyBox(const std::string& name);

    SkyBox(const SkyBox&) = delete;
    SkyBox(SkyBox&&) = delete;

    SkyBox& operator=(const SkyBox&) = delete;
    SkyBox& operator=(SkyBox&&) = delete;

    virtual ~SkyBox() = default;

    /**
     * @brief      Gets background texture
     *
     * @return     The background texture
     */
    const Resource::SharedPtr<lug::Graphics::Render::Texture> getBackgroundTexture() const;
    /**
     * @brief      Gets environment texture
     *
     * @return     The environment texture
     */
    const Resource::SharedPtr<lug::Graphics::Render::Texture> getEnvironnementTexture() const;

    /**
     * @brief      Create an irradiance map from the environment texture
     *
     * @return     The irradiance map
     */
    virtual Resource::SharedPtr<lug::Graphics::Render::SkyBox> createIrradianceMap(Renderer& renderer) const = 0;
    /**
     * @brief      Create a prefiltered map from the environment texture
     *
     * @return     The prefiltered map
     */
    virtual Resource::SharedPtr<lug::Graphics::Render::SkyBox> createPrefilteredMap(Renderer& renderer) const = 0;

protected:
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _backgroundTexture;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _environnementTexture;
};

#include <lug/Graphics/Render/SkyBox.inl>

} // Render
} // Graphics
} // lug
