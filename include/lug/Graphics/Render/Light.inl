inline Light::Type Light::getType() const {
    return _type;
}

inline void Light::setType(Type type) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _type = type;
}

inline void Light::setColor(const Math::Vec4f& color) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _color = color;
}

inline void Light::setDirection(const Math::Vec3f& direction) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _direction = direction;
}

inline void Light::setConstantAttenuation(float constantAttenuation) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _constantAttenuation = constantAttenuation;
}

inline void Light::setDistance(float distance) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _distance = distance;
}

inline void Light::setLinearAttenuation(float linearAttenuation) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _linearAttenuation = linearAttenuation;
}

inline void Light::setQuadraticAttenuation(float quadraticAttenuation) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _quadraticAttenuation = quadraticAttenuation;
}

inline void Light::setFalloffAngle(float falloffAngle) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _falloffAngle = falloffAngle;
}

inline void Light::setFalloffExponent(float falloffExponent) {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _falloffExponent = falloffExponent;
}

inline float Light::getDistance() const {
    return _distance;
}
