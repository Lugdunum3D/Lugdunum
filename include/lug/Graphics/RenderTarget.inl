inline const std::vector<std::unique_ptr<RenderView>>& RenderTarget::getRenderViews() const {
    return _renderViews;
}

inline std::vector<std::unique_ptr<RenderView>>& RenderTarget::getRenderViews() {
    return _renderViews;
}
