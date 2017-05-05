#pragma once

#include <vector>
#include <string>

#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Render/Material.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API Material {
public:
    explicit Material(Renderer& renderer);

    Material(const Material&) = default;
    Material(Material&&) = default;

    Material& operator=(const Material&) = default;
    Material& operator=(Material&&) = default;

    virtual ~Material() = default;

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the mesh.
     */
    void setName(const std::string& name);

    void setBaseColorFactor(const Math::Vec4f& factor);
    void setEmissiveFactor(const Math::Vec3f& factor);
    void setMetallicFactor(float factor);
    void setRoughnessFactor(float factor);

    // TODO(Nokitoo): replace textureName with gltf2-loader texture struct ?
    void setBaseColorTexture(const std::string& textureName, uint32_t texCoord);
    void setMetallicRoughnessTexture(const std::string& textureName, uint32_t texCoord);
    void setNormalTexture(const std::string& textureName, uint32_t texCoord, float scale = 1.0f);
    void setOcclusionTexture(const std::string& textureName, uint32_t texCoord, float strength = 1.0f);
    void setEmissiveTexture(const std::string& textureName, uint32_t texCoord);

    virtual Resource::SharedPtr<Render::Material> build() = 0;

protected:
    Renderer& _renderer;

    std::string _name;
    Render::Material::Factors _factors;
    Render::Material::TextureInfo _baseColorTexture;
    Render::Material::TextureInfo _metallicRoughnessTexture;
    Render::Material::NormalTextureInfo _normalTexture;
    Render::Material::OcclusionTextureInfo _occlusionTexture;
    Render::Material::TextureInfo _emissiveTexture;
};

} // Builder
} // Graphics
} // lug
