inline void ImageView::setFormat(VkFormat format) {
    _format = format;
}

inline void ImageView::setViewType(VkImageViewType viewType) {
    _viewType = viewType;
}

inline void ImageView::setAspectFlags(VkImageAspectFlags aspectFlags) {
    _aspectFlags = aspectFlags;
}

inline void ImageView::setLayerCount(uint32_t layerCount) {
    _layerCount = layerCount;
}

inline void ImageView::setLevelCount(uint32_t levelCount) {
    _levelCount = levelCount;
}
