inline void Graphic::addModule(Module::Type type) {
    _mandatoryModules.insert(type);
}

inline void Graphic::addOptionnalModule(Module::Type type) {
    _optionnalModules.insert(type);
}

inline void Graphic::removeModule(Module::Type type) {
    _mandatoryModules.erase(type);
}

inline void Graphic::removeOptionnalModule(Module::Type type) {
    _optionnalModules.erase(type);
}

inline const std::set<Module::Type>& Graphic::getMandatoryModules() const {
    return _mandatoryModules;
}

inline const std::set<Module::Type>& Graphic::getOptionnalModules() const {
    return _optionnalModules;
}

inline bool Graphic::isModuleLoaded(Module::Type type) const {
    return std::find(std::begin(_loadedModules), std::end(_loadedModules), type) != std::end(_loadedModules);
}

inline const std::set<Module::Type>& Graphic::getLoadedModules() const {
    return _loadedModules;
}

inline const Core::Application::Info& Graphic::getAppInfo() const {
    return _appInfo;
}
