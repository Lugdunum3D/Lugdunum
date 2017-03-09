inline void Swapchain::setOutOfDate(bool outOfDate) {
    _outOfDate = outOfDate;
}

inline bool Swapchain::isOutOfDate() const {
    return _outOfDate;
}

inline std::vector<Image>& Swapchain::getImages() {
    return _images;
}

inline const std::vector<std::unique_ptr<ImageView>>& Swapchain::getImagesViews() const {
    return _imagesViews;
}

inline const VkSurfaceFormatKHR& Swapchain::getFormat() const {
    return _format;
}

inline const VkExtent2D& Swapchain::getExtent() const {
    return _extent;
}
