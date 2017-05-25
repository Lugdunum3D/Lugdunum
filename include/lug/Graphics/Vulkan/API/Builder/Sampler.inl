inline void Sampler::setMagFilter(VkFilter magFilter) {
    _magFilter = magFilter;
}

inline void Sampler::setMinFilter(VkFilter minFilter) {
    _minFilter = minFilter;
}

inline void Sampler::setMipmapMode(VkSamplerMipmapMode mipmapMode) {
    _mipmapMode = mipmapMode;
}

inline void Sampler::setAddressModeU(VkSamplerAddressMode addressModeU) {
    _addressModeU = addressModeU;
}

inline void Sampler::setAddressModeV(VkSamplerAddressMode addressModeV) {
    _addressModeV = addressModeV;
}

inline void Sampler::setAddressModeW(VkSamplerAddressMode addressModeW) {
    _addressModeW = addressModeW;
}

inline void Sampler::setMipLodBias(float mipLodBias) {
    _mipLodBias = mipLodBias;
}

inline void Sampler::setAnisotropyEnable(VkBool32 anisotropyEnable) {
    _anisotropyEnable = anisotropyEnable;
}

inline void Sampler::setMaxAnisotropy(float maxAnisotropy) {
    _maxAnisotropy = maxAnisotropy;
}

inline void Sampler::setCompareOp(VkCompareOp compareOp) {
    _compareOp = compareOp;
}

inline void Sampler::setMinLod(float minLod) {
    _minLod = minLod;
}

inline void Sampler::setMaxLod(float maxLod) {
    _maxLod = maxLod;
}

inline void Sampler::setBorderColor(VkBorderColor borderColor) {
    _borderColor = borderColor;
}

inline void Sampler::setUnnormalizedCoordinates(VkBool32 unnormalizedCoordinates) {
    _unnormalizedCoordinates = unnormalizedCoordinates;
}

