#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
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
class LUG_GRAPHICS_API Material : public Resource {
    friend class ::lug::Graphics::Builder::Material;

public:
    struct TextureInfo {
        Resource::SharedPtr<Resource> texture; // TODO: Change the template type to Texture
        uint32_t texCoord{0};
    };

    struct Constants {
        Math::Vec4f baseColor{1.0f};            ///< Overall color of the material.
        Math::Vec3f emissive{0.0f};             ///< Color of the emissive factor.
        float metallic{0.0f};                   ///< How metal-like the material is. 0 is non-metal (e.g. wood, plastic), 1 is metallic.
        float roughness{0.0f};                  ///< How rough the material is. 0 is smooth and 1 is rough (matte).
        float normalTextureScale{1.0f};         ///< Scale of the normal texture
        float occlusionTextureStrength{1.0f};   ///< Strength of the occlusion texture
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

    const std::string& getName() const;
    void setName(const std::string &name);

protected:
    std::string _name;

    Constants _constants;
    TextureInfo _baseColorTexture;
    TextureInfo _metallicRoughnessTexture;
    TextureInfo _normalTexture;
    TextureInfo _occlusionTexture;
    TextureInfo _emissiveTexture;
};

#include <lug/Graphics/Render/Material.inl>

} // Render
} // Graphics
} // lug
