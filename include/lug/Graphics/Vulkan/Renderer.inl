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

inline const API::Device& Renderer::getDevice() const {
    return _device;
}

inline std::vector<API::Queue> &Renderer::getQueues() {
    return _queues;
}

inline const std::vector<API::Queue>& Renderer::getQueues() const {
    return _queues;
}

inline API::Queue* Renderer::getQueue(VkQueueFlags flags, bool supportPresentation) {
    API::Queue* returnQueue = nullptr;

    for (auto& queue : _queues) {
        if ((queue.getFlags() & flags) == flags && (!supportPresentation || queue.supportsPresentation())) {
            if (returnQueue == nullptr || queue.getFlags() == flags || (supportPresentation && flags == 0 && queue.getFlags() & VK_QUEUE_GRAPHICS_BIT)) {
                returnQueue = &queue;
            }
        }
    }

    if (!returnQueue || flags == VK_QUEUE_TRANSFER_BIT) {
        return getQueue(VK_QUEUE_GRAPHICS_BIT, supportPresentation);
    }

    return returnQueue;
}

inline const API::Queue* Renderer::getQueue(VkQueueFlags flags, bool supportPresentation) const {
    const API::Queue* returnQueue = nullptr;

    for (const auto& queue : _queues) {
        if ((queue.getFlags() & flags) == flags && (!supportPresentation || queue.supportsPresentation())) {
            if (returnQueue == nullptr || queue.getFlags() == flags || (supportPresentation && flags == 0 && queue.getFlags() & VK_QUEUE_GRAPHICS_BIT)) {
                returnQueue = &queue;
            }
        }
    }

    if (!returnQueue || flags == VK_QUEUE_TRANSFER_BIT) {
        return getQueue(VK_QUEUE_GRAPHICS_BIT, supportPresentation);
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
