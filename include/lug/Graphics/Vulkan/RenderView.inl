inline RenderTechnique* RenderView::getRenderTechnique() {
    return _renderTechnique.get();
}

inline const Semaphore& RenderView::getDrawCompleteSemaphore() const {
    return _drawCompleteSemaphore;
}
