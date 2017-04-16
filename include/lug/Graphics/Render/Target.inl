inline const std::vector<std::unique_ptr<View>>& Target::getRenderViews() const {
    return _renderViews;
}

inline std::vector<std::unique_ptr<View>>& Target::getRenderViews() {
    return _renderViews;
}
