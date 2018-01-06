inline const Renderer::InitInfo& Renderer::getInfo() const {
    return _initInfo;
}

inline Renderer::Type Renderer::getType() const {
    return _type;
}

inline ResourceManager* Renderer::getResourceManager() const {
    return _resourceManager.get();
}

inline const Renderer::DisplayMode& Renderer::getDisplayMode() const {
    return _displayMode;
}

inline void Renderer::setDisplayMode(Renderer::DisplayMode displayMode) {
    _displayMode = displayMode;
}

inline const Renderer::Antialiasing& Renderer::getAntialiasing() const {
    return _antialiasing;
}

inline void Renderer::setAntialiasing(Renderer::Antialiasing antialiasing) {
    _antialiasing = antialiasing;
}

inline bool Renderer::isBloomEnabled() const {
    return _bloomEnabled;
}

inline void Renderer::isBloomEnabled(bool enabled) {
    _bloomEnabled = enabled;
}

inline bool Renderer::isBloomDirty() const {
    return _bloomDirty;
}

inline void Renderer::isBloomDirty(bool dirty) {
    _bloomDirty = dirty;
}

inline float Renderer::getBlurThreshold() const {
    return _bloomOptions.blurThreshold;
}

inline void Renderer::setBlurThreshold(float blurThreshold) {
    _bloomOptions.blurThreshold = blurThreshold;
    isBloomDirty(true);
}
