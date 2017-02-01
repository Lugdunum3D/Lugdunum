inline const Swapchain& RenderWindow::getSwapchain() const {
    return _swapchain;
}

inline uint16_t RenderWindow::getWidth() const {
    return _mode.width;
}

inline uint16_t RenderWindow::getHeight() const {
    return _mode.height;
}
