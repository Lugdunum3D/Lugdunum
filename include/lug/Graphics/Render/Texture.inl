inline void Texture::setWidth(uint32_t width) {
    _width = width;
}

inline void Texture::setHeight(uint32_t height) {
    _height = height;
}

inline Render::Texture::Filter Texture::getMagFilter() const {
    return _magFilter;
}

inline Render::Texture::Filter Texture::getMinFilter() const {
    return _minFilter;
}

inline Render::Texture::Filter Texture::getMipMapFilter() const {
    return _mipMapFilter;
}

inline Render::Texture::WrappingMode Texture::getWrapS() const {
    return _wrapS;
}

inline Render::Texture::WrappingMode Texture::getWrapT() const {
    return _wrapT;
}

inline uint32_t Texture::getWidth() const {
    return _width;
}

inline uint32_t Texture::getHeight() const {
    return _height;
}
