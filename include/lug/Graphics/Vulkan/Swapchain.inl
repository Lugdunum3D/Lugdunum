inline void Swapchain::setOutOfDate(bool outOfDate) {
    _outOfDate = outOfDate;
}

inline bool Swapchain::isOutOfDate() const {
    return _outOfDate;
}

inline std::vector<Image>& Swapchain::getImages() {
    return _images;
}

inline const std::vector<Framebuffer>& Swapchain::getFramebuffers() const {
    return _framebuffers;
}

inline const VkSurfaceFormatKHR& Swapchain::getFormat() const {
    return _format;
}

inline const VkExtent2D& Swapchain::getExtent() const {
    return _extent;
}