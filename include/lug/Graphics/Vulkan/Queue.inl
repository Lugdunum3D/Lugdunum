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
