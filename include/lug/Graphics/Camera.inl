inline Scene* Camera::getScene() const {
    return _scene;
}

inline void Camera::setScene(Scene* scene) {
    _scene = scene;
}

inline RenderQueue& Camera::getRenderQueue() {
    return _renderQueue;
}

inline const RenderQueue& Camera::getRenderQueue() const {
    return _renderQueue;
}

inline float Camera::getFov() const {
    return _fov;
}

inline void Camera::setFov(float fov) {
    _fov = fov;
    needUpdateProj();
}

inline float Camera::getFar() const {
    return _far;
}

inline void Camera::setFar(float farDist) {
    _far = farDist;
    needUpdateProj();
}

inline float Camera::getNear() const {
    return _near;
}

inline void Camera::setNear(float nearDist) {
    _near = nearDist;
    needUpdateProj();
}

inline const Math::Mat4x4f& Camera::getProjectionMatrix() {
    return _projMatrix;
}
