inline Technique::Technique* View::getRenderTechnique() {
    return _renderTechnique.get();
}

inline const API::Semaphore& View::getDrawCompleteSemaphore(uint32_t currentImageIndex) const {
    return _drawCompleteSemaphores[currentImageIndex];
}
