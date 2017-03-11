inline const API::Swapchain& Window::getSwapchain() const {
    return _swapchain;
}

inline uint16_t Window::getWidth() const {
    return _mode.width;
}

inline uint16_t Window::getHeight() const {
    return _mode.height;
}
