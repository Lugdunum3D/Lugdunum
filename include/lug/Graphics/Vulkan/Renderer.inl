inline bool Renderer::isInstanceLayerLoaded(const char* name) const {
    return std::find(_loadedInstanceLayers.cbegin(), _loadedInstanceLayers.cend(), name) != _loadedInstanceLayers.cend();
}

inline bool Renderer::isInstanceExtensionLoaded(const char* name) const {
    return std::find(_loadedInstanceExtensions.cbegin(), _loadedInstanceExtensions.cend(), name) != _loadedInstanceExtensions.cend();
}

inline bool Renderer::isDeviceExtensionLoaded(const char* name) const {
    return std::find(_loadedDeviceExtensions.cbegin(), _loadedDeviceExtensions.cend(), name) != _loadedDeviceExtensions.cend();
}

inline const Instance& Renderer::getInstance() const {
    return _instance;
}

inline const Device& Renderer::getDevice() const {
    return _device;
}

inline std::vector<Queue> &Renderer::getQueues() {
    return _queues;
}

inline const std::vector<Queue>& Renderer::getQueues() const {
    return _queues;
}

inline const Queue* Renderer::getQueue(VkQueueFlags flags, bool supportPresentation) const {
    const Queue* returnQueue = nullptr;

    for (const auto& queue : _queues) {
        if ((queue.getFlags() & flags) == flags && (!supportPresentation || queue.supportsPresentation())) {
            if (returnQueue == nullptr || queue.getFlags() == flags) {
                returnQueue = &queue;
            }
        }
    }

    return returnQueue;
}

inline bool Renderer::isSameQueue(VkQueueFlags flagsA, bool supportPresentationA, VkQueueFlags flagsB, bool supportPresentationB) const {
    return getQueue(flagsA, supportPresentationA) == getQueue(flagsB, supportPresentationB);
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
