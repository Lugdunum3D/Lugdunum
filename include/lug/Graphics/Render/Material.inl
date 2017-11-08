inline void Material::setIrradianceMap(const Resource::SharedPtr<SkyBox> irradianceMap) {
    _irradianceMap = irradianceMap;
    DirtyObject::setDirty(true);
}

inline void Material::setPrefilteredMap(const Resource::SharedPtr<SkyBox> prefilteredMap) {
    _prefilteredMap = prefilteredMap;
    DirtyObject::setDirty(true);
}

inline const Material::Constants& Material::getConstants() const {
    return _constants;
}

inline const Material::TextureInfo& Material::getBaseColorTexture() const {
    return _baseColorTexture;
}

inline const Material::TextureInfo& Material::getMetallicRoughnessTexture() const {
    return _metallicRoughnessTexture;
}

inline const Material::TextureInfo& Material::getNormalTexture() const {
    return _normalTexture;
}

inline const Material::TextureInfo& Material::getOcclusionTexture() const {
    return _occlusionTexture;
}

inline const Material::TextureInfo& Material::getEmissiveTexture() const {
    return _emissiveTexture;
}

inline const Resource::SharedPtr<SkyBox> Material::getIrradianceMap() const {
    return _irradianceMap;
}

inline const Resource::SharedPtr<SkyBox> Material::getPrefilteredMap() const {
    return _prefilteredMap;
}
