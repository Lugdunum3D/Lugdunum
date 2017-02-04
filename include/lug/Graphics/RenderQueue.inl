inline const std::vector<MovableObject*>& RenderQueue::getObjects() const {
    return _queue;

}

inline std::size_t RenderQueue::getObjectsNb() const {
    return _size;
}

inline const std::vector<Light*>& RenderQueue::getLights() const {
    return _lightsQueue;

}

inline std::size_t RenderQueue::getLightsNb() const {
    return _lightsNb;
}
