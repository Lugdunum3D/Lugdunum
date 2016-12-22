inline int8_t Queue::getFamilyIdx() const {
    return _idx;
}

inline VkQueueFlags Queue::getFlags() const {
    return _flags;
}

inline bool Queue::supportsPresentation() const {
    return _presentation;
}

inline void Queue::supportsPresentation(bool presentation) {
    _presentation = presentation;
}

inline CommandPool& Queue::getCommandPool() {
    return _commandPool;
}

inline const CommandPool& Queue::getCommandPool() const {
    return _commandPool;
}

inline void Queue::setCommandPool(CommandPool&& commandPool) {
    _commandPool = std::move(commandPool);
}
