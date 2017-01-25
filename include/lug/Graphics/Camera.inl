inline Scene* Camera::getScene() const {
    return _scene;
}

inline RenderQueue& Camera::getRenderQueue() {
    return _renderQueue;
}

inline const RenderQueue& Camera::getRenderQueue() const {
    return _renderQueue;
}
