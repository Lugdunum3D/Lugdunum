inline float Perspective::getFovY() const {
    return _fovy;
}

inline void Perspective::setFovY(float fovy) {
    _fovy = fovy;
    needUpdateProj();
}

inline float Perspective::getAspectRatio() const {
    return _aspectRatio;
}

inline void Perspective::setAspectRatio(float aspectRatio) {
    _aspectRatio = aspectRatio;
    needUpdateProj();
}
