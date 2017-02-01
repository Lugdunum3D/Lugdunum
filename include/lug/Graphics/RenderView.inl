inline RenderView::InitInfo& RenderView::getInfo() {
    return _info;
}

inline const RenderView::InitInfo& RenderView::getInfo() const {
    return _info;
}

inline const RenderView::Viewport& RenderView::getViewport() const {
    return _viewport;
}

inline const RenderView::Scissor& RenderView::getScissor() const {
    return _scissor;
}

inline std::unique_ptr<Camera> RenderView::attachCamera(std::unique_ptr<Camera> camera) {
    std::unique_ptr<Camera> old = std::move(_camera);
    _camera = std::move(camera);
    return old;
}

inline std::unique_ptr<Camera> RenderView::detachCamera() {
    return std::move(_camera);
}

inline Camera* RenderView::getCamera() const {
    return _camera.get();
}


inline float RenderView::Viewport::getRatio() const {
    return extent.width / extent.height;
}
