inline uint32_t Texture::getWidth() const {
    return _width;
}

inline uint32_t Texture::getHeight() const {
    return _height;
}

inline Render::Texture::Format Texture::getFormat() const {
    return _format;
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

inline size_t Texture::formatToSize(Render::Texture::Format format) {
    switch(format) {
        case Texture::Format::R8G8B8A8_UNORM:
        case Texture::Format::R16G16_SFLOAT:
            return 4;
        case Texture::Format::R16G16B16_SFLOAT:
            return 6;
        case Texture::Format::R32G32B32A32_SFLOAT:
            return 16;
        default:
            return 0;
    };
}
