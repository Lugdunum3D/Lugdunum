inline void SkyBox::SkyBox::setName(const std::string& name) {
    _name = name;
}

inline void SkyBox::setMagFilter(Render::Texture::Filter magFilter) {
    _magFilter = magFilter;
}

inline void SkyBox::setMinFilter(Render::Texture::Filter minFilter) {
    _minFilter = minFilter;
}

inline void SkyBox::setMipMapFilter(Render::Texture::Filter mipMapFilter) {
    _mipMapFilter = mipMapFilter;
}

inline void SkyBox::setWrapS(Render::Texture::WrappingMode wrapS) {
    _wrapS = wrapS;
}

inline void SkyBox::setWrapT(Render::Texture::WrappingMode wrapT) {
    _wrapT = wrapT;
}

inline void SkyBox::setWrapW(Render::Texture::WrappingMode wrapW) {
    _wrapW = wrapW;
}

inline void SkyBox::setFaceFilename(Face face, const std::string& filename) {
    _faces[static_cast<uint8_t>(face)] = filename;
}
