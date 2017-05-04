inline const Renderer::InitInfo& Renderer::getInfo() const {
    return _initInfo;
}

inline Renderer::Type Renderer::getType() const {
    return _type;
}

inline ResourceManager* Renderer::getResourceManager() const {
    return _resourceManager.get();
}
