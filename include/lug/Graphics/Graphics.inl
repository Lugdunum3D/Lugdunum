inline void Graphics::addModule(Module::Type type) {
    _mandatoryModules.insert(type);
}

inline void Graphics::addOptionalModule(Module::Type type) {
    _optionalModules.insert(type);
}

inline void Graphics::removeModule(Module::Type type) {
    _mandatoryModules.erase(type);
}

inline void Graphics::removeOptionalModule(Module::Type type) {
    _optionalModules.erase(type);
}

inline const std::set<Module::Type>& Graphics::getMandatoryModules() const {
    return _mandatoryModules;
}

inline const std::set<Module::Type>& Graphics::getOptionalModules() const {
    return _optionalModules;
}

inline bool Graphics::isModuleLoaded(Module::Type type) const {
    return std::find(std::begin(_loadedModules), std::end(_loadedModules), type) != std::end(_loadedModules);
}

inline const std::set<Module::Type>& Graphics::getLoadedModules() const {
    return _loadedModules;
}

inline const Core::Application::Info& Graphics::getAppInfo() const {
    return _appInfo;
}

inline Renderer* Graphics::getRenderer() const {
    return _renderer.get();
}
