inline float Camera::getZFar() const {
    return _zfar;
}

inline void Camera::setZFar(float zfar) {
    _zfar = zfar;
    needUpdateProj();
}

inline float Camera::getZNear() const {
    return _znear;
}

inline void Camera::setZNear(float znear) {
    _znear = znear;
    needUpdateProj();
}

inline void Camera::setRenderView(View* renderView) {
    _renderView = renderView;
    needUpdateProj();
}

inline Scene::Node* Camera::getParent() const {
    return _parent;
}

inline void Camera::setParent(Scene::Node* parent) {
    _parent = parent;
    needUpdateView();
}

inline const Math::Mat4x4f& Camera::getProjectionMatrix() {
    if (_needUpdateProj) {
        updateProj();
    }

    return _projMatrix;
}

inline const Math::Mat4x4f& Camera::getViewMatrix() {
    if (_needUpdateView) {
        updateView();
    }

    return _viewMatrix;
}

inline void Camera::needUpdateProj() {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _needUpdateProj = true;
}

inline void Camera::needUpdateView() {
    ::lug::Graphics::Render::DirtyObject::setDirty();
    _needUpdateView = true;
}
