inline bool Graphics::isModuleLoaded(Module::Type type) const {
    return (
        std::find(std::begin(_loadedMandatoryModules), std::end(_loadedMandatoryModules), type) != std::end(_loadedMandatoryModules)
        || std::find(std::begin(_loadedOptionalModules), std::end(_loadedOptionalModules), type) != std::end(_loadedOptionalModules)
    );
}

inline const std::set<Module::Type>& Graphics::getLoadedMandatoryModules() const {
    return _loadedMandatoryModules;
}

inline const std::set<Module::Type>& Graphics::getLoadedOptionalModules() const {
    return _loadedOptionalModules;
}

inline Renderer* Graphics::getRenderer() const {
    return _renderer.get();
}
