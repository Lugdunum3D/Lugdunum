inline Light::Type Light::getLightType() const {
    return _type;
}

inline void Light::setAmbient(const Math::Vec3f& ambient) {
    _ambient = ambient;
    needUpdate();
}

inline void Light::setDiffuse(const Math::Vec3f& diffuse) {
    _diffuse = diffuse;
    needUpdate();
}

inline void Light::setSpecular(const Math::Vec3f& specular) {
    _specular = specular;
    needUpdate();
}
