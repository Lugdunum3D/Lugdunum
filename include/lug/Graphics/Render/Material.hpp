#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {
namespace Render {

/**
 * @brief     Class for Material
 */
class LUG_GRAPHICS_API Material : public Resource {
public:
    struct TextureInfo {
        //Texture* texture;
        uint32_t texCoord;
    };

    struct NormalTextureInfo : public TextureInfo {
        float scale;
    };

    struct OcclusionTextureInfo : public TextureInfo {
        float strength;
    };

    struct Factors {
        Math::Vec4f baseColor;
        Math::Vec3f emissive;
        float metallic;
        float roughness;
    } _factors;

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

private:
    std::string _name;

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
