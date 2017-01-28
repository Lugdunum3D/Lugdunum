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
