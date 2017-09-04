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
    void setIrradianceMap(const Resource::SharedPtr<SkyBox> irradianceMap);

    const Constants& getConstants() const;

    const TextureInfo& getBaseColorTexture() const;
    const TextureInfo& getMetallicRoughnessTexture() const;
    const TextureInfo& getNormalTexture() const;
    const TextureInfo& getOcclusionTexture() const;
    const TextureInfo& getEmissiveTexture() const;
    const Resource::SharedPtr<SkyBox> getIrradianceMap() const;

protected:
    Constants _constants;
    TextureInfo _baseColorTexture;
    TextureInfo _metallicRoughnessTexture;
    TextureInfo _normalTexture;
    TextureInfo _occlusionTexture;
    TextureInfo _emissiveTexture;
    Resource::SharedPtr<SkyBox> _irradianceMap;
};

#include <lug/Graphics/Render/Material.inl>

} // Render
} // Graphics
} // lug
