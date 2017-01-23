inline const Swapchain& RenderWindow::getSwapchain() const {
    return _swapchain;
}

inline const Framebuffer& RenderWindow::getCurrentFramebuffer() const {
    return _swapchain.getFramebuffers()[_currentImageIndex];
}
