inline void Framebuffer::setRenderPass(const API::RenderPass* renderPass) {
    _renderPass = renderPass;
}

inline void Framebuffer::addAttachment(const API::ImageView* attachment) {
    _attachments.push_back(attachment);
}

inline void Framebuffer::setWidth(uint32_t width) {
    _width = width;
}

inline void Framebuffer::setHeight(uint32_t height) {
    _height = height;
}

inline void Framebuffer::setLayers(uint32_t layers) {
    _layers = layers;
}
