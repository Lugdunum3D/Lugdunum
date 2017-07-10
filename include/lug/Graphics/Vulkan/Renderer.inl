inline bool Renderer::isInstanceLayerLoaded(const char* name) const {
    auto compareLayers = [&name](const char* layerName) {
        return std::strcmp(layerName, name) == 0;
    };

    return std::find_if(_loadedInstanceLayers.cbegin(), _loadedInstanceLayers.cend(), compareLayers) != _loadedInstanceLayers.cend();
}

inline bool Renderer::isInstanceExtensionLoaded(const char* name) const {
    auto compareExtensions = [&name](const char* extensionName) {
        return std::strcmp(extensionName, name) == 0;
    };

    return std::find_if(_loadedInstanceExtensions.cbegin(), _loadedInstanceExtensions.cend(), compareExtensions) != _loadedInstanceExtensions.cend();
}

inline bool Renderer::isDeviceExtensionLoaded(const char* name) const {
    auto compareExtensions = [&name](const char* extensionName) {
        return std::strcmp(extensionName, name) == 0;
    };

    return std::find_if(_loadedDeviceExtensions.cbegin(), _loadedDeviceExtensions.cend(), compareExtensions) != _loadedDeviceExtensions.cend();
}

inline const API::Instance& Renderer::getInstance() const {
    return _instance;
}

inline API::Device& Renderer::getDevice() {
    return _device;
}

inline const API::Device& Renderer::getDevice() const {
    return _device;
}

inline InstanceInfo& Renderer::getInstanceInfo() {
    return _instanceInfo;
}

inline const InstanceInfo& Renderer::getInstanceInfo() const {
    return _instanceInfo;
}

inline PhysicalDeviceInfo* Renderer::getPhysicalDeviceInfo() {
    return _physicalDeviceInfo;
}

inline const PhysicalDeviceInfo* Renderer::getPhysicalDeviceInfo() const {
    return _physicalDeviceInfo;
}

inline std::vector<PhysicalDeviceInfo>& Renderer::getPhysicalDeviceInfos() {
    return _physicalDeviceInfos;
}

inline const std::vector<PhysicalDeviceInfo>& Renderer::getPhysicalDeviceInfos() const {
    return _physicalDeviceInfos;
}

inline Renderer::Preferences& Renderer::getPreferences() {
    return _preferences;
}

inline const Renderer::Preferences& Renderer::getPreferences() const {
    return _preferences;
}

inline void Renderer::addPipeline(Resource::SharedPtr<Render::Pipeline> pipeline) {
    _pipelines[pipeline->getId()] = pipeline;
}

inline bool Renderer::containsPipeline(Render::Pipeline::Id id) const {
    return _pipelines.find(id) != _pipelines.end() && _pipelines.at(id).lock();
}

inline Resource::SharedPtr<Render::Pipeline> Renderer::getPipeline(Render::Pipeline::Id id) {
    if (containsPipeline(id)) {
        return _pipelines.at(id).lock();
    }

    return Render::Pipeline::create(*this, id);
}

inline Render::Window* Renderer::getRenderWindow() const {
    return _window.get();
}
