inline uint32_t QueueFamily::getIdx() const {
    return _idx;
}

inline VkQueueFlags QueueFamily::getFlags() const {
    return _flags;
}

inline const std::vector<Queue>& QueueFamily::getQueues() const {
    return _queues;
}

inline const Queue* QueueFamily::getQueue(const std::string& queueName) const {
    auto queue = _queuesIndices.find(queueName);
    if (queue == _queuesIndices.end()) {
        return nullptr;
    }
    return &_queues[queue->second];
}

inline bool QueueFamily::supportsPresentation() const {
    return _presentation;
}

inline void QueueFamily::supportsPresentation(bool presentation) {
    _presentation = presentation;
}
