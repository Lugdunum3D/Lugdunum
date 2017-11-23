inline View::InitInfo& View::getInfo() {
    return _info;
}

inline const View::InitInfo& View::getInfo() const {
    return _info;
}

inline const View::Viewport& View::getViewport() const {
    return _viewport;
}

inline const View::Scissor& View::getScissor() const {
    return _scissor;
}

inline const Math::Vec3f& View::getClearColor() const {
    return _clearColor;
}

inline void View::setClearColor(const Math::Vec3f& color) {
    _clearColor = color;
}

inline void View::attachCamera(Resource::SharedPtr<Camera::Camera> camera) {
    if (!camera) {
        _camera = nullptr;
        return;
    }

    if (_camera) {
        _camera->setRenderView(nullptr);
    }

    _camera = std::move(camera);
    _camera->setRenderView(this);
}

inline Resource::SharedPtr<Camera::Camera> View::getCamera() const {
    return _camera;
}


inline float View::Viewport::getRatio() const {
    return extent.width / extent.height;
}
