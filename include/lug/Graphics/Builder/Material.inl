inline void Material::Material::setName(const std::string& name) {
    _name = name;
}

inline void Material::setBaseColorFactor(const Math::Vec4f& factor) {
    _constants.baseColor = factor;
}

inline void Material::setEmissiveFactor(const Math::Vec3f& factor) {
    _constants.emissive = factor;
}

inline void Material::setMetallicFactor(float factor) {
    _constants.metallic = factor;
}

inline void Material::setRoughnessFactor(float factor) {
    _constants.roughness = factor;
}

inline void Material::setBaseColorTexture(Resource::SharedPtr<Render::Texture> texture, uint32_t texCoord) {
    _baseColorTexture.texture = texture;
    _baseColorTexture.texCoord = texCoord;
}

inline void Material::setMetallicRoughnessTexture(Resource::SharedPtr<Render::Texture> texture, uint32_t texCoord) {
    _metallicRoughnessTexture.texture = texture;
    _metallicRoughnessTexture.texCoord = texCoord;
}

inline void Material::setNormalTexture(Resource::SharedPtr<Render::Texture> texture, uint32_t texCoord, float scale) {
    _normalTexture.texture = texture;
    _normalTexture.texCoord = texCoord;
    _constants.normalTextureScale = scale;
}

inline void Material::setOcclusionTexture(Resource::SharedPtr<Render::Texture> texture, uint32_t texCoord, float strength) {
    _occlusionTexture.texture = texture;
    _occlusionTexture.texCoord = texCoord;
    _constants.occlusionTextureStrength = strength;
}

inline void Material::setEmissiveTexture(Resource::SharedPtr<Render::Texture> texture, uint32_t texCoord) {
    _emissiveTexture.texture = texture;
    _emissiveTexture.texCoord = texCoord;
}
