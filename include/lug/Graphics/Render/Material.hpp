#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/DirtyObject.hpp>
#include <lug/Graphics/Render/Texture.hpp>
#include <lug/Graphics/Render/SkyBox.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

// For friend
namespace Builder {
class Material;
} // Builder

namespace Render {

/**
 * @brief     Class for Material. Is a Resource.
 */
class LUG_GRAPHICS_API Material : public Resource, public DirtyObject {
    friend class ::lug::Graphics::Builder::Material;

public:
    struct TextureInfo {
        Resource::SharedPtr<Texture> texture;
        uint32_t texCoord{0};
    };

    struct Constants {
        Math::Vec4f baseColor{1.0f, 1.0f, 1.0f, 1.0f};  ///< Overall color of the material.
        Math::Vec3f emissive{0.0f, 0.0f, 0.0f};         ///< Color of the emissive factor.
        float metallic{1.0f};                           ///< How metal-like the material is. 0 is non-metal (e.g. wood, plastic), 1 is metallic.
        float roughness{1.0f};                          ///< How rough the material is. 0 is smooth and 1 is rough (matte).
        float normalTextureScale{1.0f};                 ///< Scale of the normal texture
        float occlusionTextureStrength{1.0f};           ///< Strength of the occlusion texture
    };

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

    virtual ~Material() = default;

    // TODO: Setters which call ::lug::Graphics::Render::DirtyObject::setDirty()
    /**
     * @brief      Sets the irradiance map
     *
     * @param[in]  znear    The irradiance map
     */
    void setIrradianceMap(const Resource::SharedPtr<SkyBox> irradianceMap);
    /**
     * @brief      Sets the prefiltered map
     *
     * @param[in]  znear    The prefiltered map
     */
    void setPrefilteredMap(const Resource::SharedPtr<SkyBox> prefilteredMap);

    const Constants& getConstants() const;

    /**
     * @brief      Gets base color texture
     *
     * @return     The base color texture
     */
    const TextureInfo& getBaseColorTexture() const;
    /**
     * @brief      Gets metallic roughness texture
     *
     * @return     The metallic roughness texture
     */
    const TextureInfo& getMetallicRoughnessTexture() const;
    /**
     * @brief      Gets normal texture
     *
     * @return     The normal texture
     */
    const TextureInfo& getNormalTexture() const;
    /**
     * @brief      Gets occlusion texture
     *
     * @return     The occlusion texture
     */
    const TextureInfo& getOcclusionTexture() const;
    /**
     * @brief      Gets emissive texture
     *
     * @return     The emissive texture
     */
    const TextureInfo& getEmissiveTexture() const;
    /**
     * @brief      Gets irradiance map
     *
     * @return     The irradiance map
     */
    const Resource::SharedPtr<SkyBox> getIrradianceMap() const;
    /**
     * @brief      Gets prefiltered map
     *
     * @return     The prefiltered map
     */
    const Resource::SharedPtr<SkyBox> getPrefilteredMap() const;

protected:
    Constants _constants;
    TextureInfo _baseColorTexture;
    TextureInfo _metallicRoughnessTexture;
    TextureInfo _normalTexture;
    TextureInfo _occlusionTexture;
    TextureInfo _emissiveTexture;
    Resource::SharedPtr<SkyBox> _irradianceMap;
    Resource::SharedPtr<SkyBox> _prefilteredMap;
};

#include <lug/Graphics/Render/Material.inl>

} // Render
} // Graphics
} // lug
