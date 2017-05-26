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

    // TODO: Replace Resource::SharedPtr<Resource> texture by Resource::SharedPtr<Texture>
    void setBaseColorTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord = 0);
    void setMetallicRoughnessTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord = 0);
    void setNormalTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord = 0, float scale = 1.0f);
    void setOcclusionTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord = 0, float strength = 1.0f);
    void setEmissiveTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord = 0);

    virtual Resource::SharedPtr<Render::Material> build() = 0;

protected:
    Renderer& _renderer;

    std::string _name;
    Render::Material::Constants _constants;
    Render::Material::TextureInfo _baseColorTexture;
    Render::Material::TextureInfo _metallicRoughnessTexture;
    Render::Material::TextureInfo _normalTexture;
    Render::Material::TextureInfo _occlusionTexture;
    Render::Material::TextureInfo _emissiveTexture;
};

#include <lug/Graphics/Builder/Material.inl>

} // Builder
} // Graphics
} // lug
