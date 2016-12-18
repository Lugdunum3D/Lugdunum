inline bool Renderer::isInstanceLayerLoaded(const char* name) const {
    return std::find(_loadedInstanceLayers.cbegin(), _loadedInstanceLayers.cend(), name) != _loadedInstanceLayers.cend();
}

inline bool Renderer::isInstanceExtensionLoaded(const char* name) const {
    return std::find(_loadedInstanceExtensions.cbegin(), _loadedInstanceExtensions.cend(), name) != _loadedInstanceExtensions.cend();
}

inline bool Renderer::isDeviceExtensionLoaded(const char* name) const {
    return std::find(_loadedDeviceExtensions.cbegin(), _loadedDeviceExtensions.cend(), name) != _loadedDeviceExtensions.cend();
}
