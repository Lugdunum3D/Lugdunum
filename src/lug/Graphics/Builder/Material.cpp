#include <lug/Graphics/Builder/Material.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Material::Material(Renderer& renderer) : _renderer(renderer) {}

void Material::Material::setName(const std::string& name) {
    _name = name;
}

void Material::setBaseColorFactor(const Math::Vec4f& factor) {
    _factors.baseColor = factor;
}

void Material::setEmissiveFactor(const Math::Vec3f& factor) {
    _factors.emissive = factor;
}

void Material::setMetallicFactor(float factor) {
    _factors.metallic = factor;
}

void Material::setRoughnessFactor(float factor) {
    _factors.roughness = factor;
}

void Material::setBaseColorTexture(const std::string& textureName, uint32_t texCoord) {
    (void)textureName;
    _baseColorTexture.texCoord = texCoord;
    // TODO(Nokitoo): add texture name
}

void Material::setMetallicRoughnessTexture(const std::string& textureName, uint32_t texCoord) {
    (void)textureName;
    _metallicRoughnessTexture.texCoord = texCoord;
    // TODO(Nokitoo): add texture name
}

void Material::setNormalTexture(const std::string& textureName, uint32_t texCoord, float scale) {
    (void)textureName;
    _normalTexture.texCoord = texCoord;
    _normalTexture.scale = scale;
    // TODO(Nokitoo): add texture name
}

void Material::setOcclusionTexture(const std::string& textureName, uint32_t texCoord, float strength) {
    (void)textureName;
    _occlusionTexture.texCoord = texCoord;
    _occlusionTexture.strength = strength;
    // TODO(Nokitoo): add texture name
}

void Material::setEmissiveTexture(const std::string& textureName, uint32_t texCoord) {
    (void)textureName;
    _emissiveTexture.texCoord = texCoord;
    // TODO(Nokitoo): add texture name
}

} // Builder
} // Graphics
} // lug
