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

inline std::unique_ptr<Camera> View::attachCamera(std::unique_ptr<Camera> camera) {
    std::unique_ptr<Camera> old = std::move(_camera);

    _camera = std::move(camera);
    _camera->setRenderView(this);


    if (old) {
        old->setRenderView(nullptr);
    }

    return old;
}

inline std::unique_ptr<Camera> View::detachCamera() {
    if (_camera) {
        _camera->setRenderView(nullptr);
    }

    return std::move(_camera);
}

inline Camera* View::getCamera() const {
    return _camera.get();
}


inline float View::Viewport::getRatio() const {
    return extent.width / extent.height;
}
