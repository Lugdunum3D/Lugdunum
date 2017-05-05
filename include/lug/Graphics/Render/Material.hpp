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
    /**
     * @brief      Handle of a material. Describes how is the material composed,
     *             with textures, no textures, etc, to be used to construct unique
     *             pipelines.
     */
    struct Handle {
        union {
            struct {
                uint32_t baseColorInfo : 2;             ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                uint32_t metallicRoughnessInfo : 2;     ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                uint32_t normalInfo : 2;                ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                uint32_t occlusionInfo : 2;             ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                uint32_t emissiveInfo : 2;              ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
            };

            uint32_t value;
        };

        explicit operator uint32_t() {
            return value;
        }
    };

    struct TextureInfo {
        //Texture* texture;
        uint32_t texCoord;
    };

    struct Factors {
        Math::Vec4f baseColor;  ///< Overall color of the material.
        Math::Vec3f emissive;   ///< Color of the emissive factor.
        float metallic;         ///< How metal-like the material is. 0 is non-metal (e.g. wood, plastic), 1 is metallic.
        float roughness;        ///< How rough the material is. 0 is smooth and 1 is rough (matte).
    };

    struct NormalTextureInfo : public TextureInfo {
        float scale;
    };

    struct OcclusionTextureInfo : public TextureInfo {
        float strength;
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

    Factors _factors;
    TextureInfo _baseColorTexture;
    TextureInfo _metallicRoughnessTexture;
    NormalTextureInfo _normalTexture;
    OcclusionTextureInfo _occlusionTexture;
    TextureInfo _emissiveTexture;
};

#include <lug/Graphics/Render/Material.inl>

} // Render
} // Graphics
} // lug
