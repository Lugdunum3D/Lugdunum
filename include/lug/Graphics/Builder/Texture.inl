inline void Texture::Texture::setName(const std::string& name) {
    _name = name;
}

inline void Texture::setType(Type type) {
    _type = type;
}

inline void Texture::setMagFilter(Render::Texture::Filter magFilter) {
    _magFilter = magFilter;
}

inline void Texture::setMinFilter(Render::Texture::Filter minFilter) {
    _minFilter = minFilter;
}

inline void Texture::setMipMapFilter(Render::Texture::Filter mipMapFilter) {
    _mipMapFilter = mipMapFilter;
}

inline void Texture::setWrapS(Render::Texture::WrappingMode wrapS) {
    _wrapS = wrapS;
}

inline void Texture::setWrapT(Render::Texture::WrappingMode wrapT) {
    _wrapT = wrapT;
}

inline void Texture::addLayer(const std::string& filename) {
    _layers.push_back({filename});
}

inline void Texture::addLayer(uint32_t width, uint32_t height, unsigned char* const data) {
    _layers.push_back({"", data, width, height});
}
