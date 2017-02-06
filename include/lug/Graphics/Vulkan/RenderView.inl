inline RenderTechnique* RenderView::getRenderTechnique() {
    return _renderTechnique.get();
}

inline const Semaphore& RenderView::getDrawCompleteSemaphore(uint32_t currentImageIndex) const {
    return _drawCompleteSemaphores[currentImageIndex];
}
