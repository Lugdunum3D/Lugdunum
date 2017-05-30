inline void Swapchain::setPreferences(const Renderer::Preferences::Swapchain& preferences) {
    _preferences = &preferences;
}

inline void Swapchain::setSurface(VkSurfaceKHR surface) {
    _surface = surface;
}

inline void Swapchain::setMinImageCount(uint32_t minImageCount) {
    _minImageCount = minImageCount;
}

inline void Swapchain::setImageFormat(VkFormat imageFormat) {
    _imageFormat = imageFormat;
}

inline void Swapchain::setImageColorSpace(VkColorSpaceKHR imageColorSpace) {
    _imageColorSpace = imageColorSpace;
}

inline void Swapchain::setImageExtent(VkExtent2D imageExtent) {
    _imageExtent = imageExtent;
}

inline void Swapchain::setImageUsage(VkImageUsageFlags imageUsage) {
    _imageUsage = imageUsage;
}

inline void Swapchain::setExclusive(bool exclusive) {
    _exclusive = exclusive;
}

inline void Swapchain::setQueueFamilyIndices(const std::set<uint32_t>& queueFamilyIndices) {
    _queueFamilyIndices = queueFamilyIndices;
}

inline void Swapchain::setPreTransform(VkSurfaceTransformFlagBitsKHR preTransform) {
    _preTransform = preTransform;
}

inline void Swapchain::setCompositeAlpha(VkCompositeAlphaFlagBitsKHR compositeAlpha) {
    _compositeAlpha = compositeAlpha;
}

inline void Swapchain::setPresentMode(VkPresentModeKHR presentMode) {
    _presentMode = presentMode;
}

inline void Swapchain::setClipped(VkBool32 clipped) {
    _clipped = clipped;
}

inline void Swapchain::setOldSwapchain(VkSwapchainKHR oldSwapchain) {
    _oldSwapchain = oldSwapchain;
}
