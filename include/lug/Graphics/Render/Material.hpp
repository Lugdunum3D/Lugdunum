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

    struct Factors {
        Math::Vec4f baseColor{1.0f};    ///< Overall color of the material.
        Math::Vec3f emissive{0.0f};     ///< Color of the emissive factor.
        float metallic{0.0f};           ///< How metal-like the material is. 0 is non-metal (e.g. wood, plastic), 1 is metallic.
        float roughness{0.0f};          ///< How rough the material is. 0 is smooth and 1 is rough (matte).
    };

    struct NormalTextureInfo : public TextureInfo {
        float scale{0.0f};
    };

    struct OcclusionTextureInfo : public TextureInfo {
        float strength{0.0f};
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
