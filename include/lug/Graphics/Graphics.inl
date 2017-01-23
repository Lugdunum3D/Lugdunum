inline bool Graphics::isModuleLoaded(Module::Type type) const {
    return std::find(std::begin(_loadedModules), std::end(_loadedModules), type) != std::end(_loadedModules);
}

inline const std::set<Module::Type>& Graphics::getLoadedModules() const {
    return _loadedModules;
}

inline Renderer* Graphics::getRenderer() const {
    return _renderer.get();
}
