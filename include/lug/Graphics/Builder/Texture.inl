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

inline void Texture::setWrapW(Render::Texture::WrappingMode wrapW) {
    _wrapW = wrapW;
}
