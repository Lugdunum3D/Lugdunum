inline void Graphics::addModule(Module::Type type) {
    _mandatoryModules.insert(type);
}

inline void Graphics::addOptionnalModule(Module::Type type) {
    _optionnalModules.insert(type);
}

inline void Graphics::removeModule(Module::Type type) {
    _mandatoryModules.erase(type);
}

inline void Graphics::removeOptionnalModule(Module::Type type) {
    _optionnalModules.erase(type);
}

inline const std::set<Module::Type>& Graphics::getMandatoryModules() const {
    return _mandatoryModules;
}

inline const std::set<Module::Type>& Graphics::getOptionnalModules() const {
    return _optionnalModules;
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
