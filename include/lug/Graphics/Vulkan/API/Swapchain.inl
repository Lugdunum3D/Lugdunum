inline void Swapchain::setOutOfDate(bool outOfDate) {
    _outOfDate = outOfDate;
}

inline bool Swapchain::isOutOfDate() const {
    return _outOfDate;
}

inline const std::vector<Image>& Swapchain::getImages() const {
    return _images;
}

inline const std::vector<ImageView>& Swapchain::getImagesViews() const {
    return _imagesViews;
}

inline const VkSurfaceFormatKHR& Swapchain::getFormat() const {
    return _format;
}

inline const VkExtent2D& Swapchain::getExtent() const {
    return _extent;
}
