inline void Instance::setApplicationInfo(const std::string& name, const Core::Version& version) {
    _appInfo.appName = name;
    _appInfo.appVersion = version;
}

inline void Instance::setEngineInfo(const std::string& name, const Core::Version& version) {
    _appInfo.engineName = name;
    _appInfo.engineVersion = version;
}

inline void Instance::setRequiredVulkanVersion(const Core::Version& version) {
    _appInfo.apiVersion = version;
}

inline void Instance::setLayers(const std::vector<const char*>& layers) {
    _layers = layers;
}

inline void Instance::setExtensions(const std::vector<const char*>& extensions) {
    _extensions = extensions;
}
