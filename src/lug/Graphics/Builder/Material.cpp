#include <lug/Graphics/Builder/Material.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Material::Material(Renderer& renderer) : _renderer(renderer) {}

void Material::Material::setName(const std::string& name) {
    _name = name;
}

void Material::setBaseColorFactor(const Math::Vec4f& factor) {
    _constants.baseColor = factor;
}

void Material::setEmissiveFactor(const Math::Vec3f& factor) {
    _constants.emissive = factor;
}

void Material::setMetallicFactor(float factor) {
    _constants.metallic = factor;
}

void Material::setRoughnessFactor(float factor) {
    _constants.roughness = factor;
}

void Material::setBaseColorTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord) {
    _baseColorTexture.texture = texture;
    _baseColorTexture.texCoord = texCoord;
}

void Material::setMetallicRoughnessTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord) {
    _metallicRoughnessTexture.texture = texture;
    _metallicRoughnessTexture.texCoord = texCoord;
}

void Material::setNormalTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord, float scale) {
    _normalTexture.texture = texture;
    _normalTexture.texCoord = texCoord;
    _constants.normalTextureScale = scale;
}

void Material::setOcclusionTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord, float strength) {
    _occlusionTexture.texture = texture;
    _occlusionTexture.texCoord = texCoord;
    _constants.occlusionTextureStrength = strength;
}

void Material::setEmissiveTexture(Resource::SharedPtr<Resource> texture, uint32_t texCoord) {
    _emissiveTexture.texture = texture;
    _emissiveTexture.texCoord = texCoord;
}

} // Builder
} // Graphics
} // lug
