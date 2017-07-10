inline const API::DeviceMemory& Texture::getDeviceMemory() const {
    return _deviceMemory;
}

inline const API::Image& Texture::getImage() const {
    return _image;
}

inline const API::ImageView& Texture::getImageView() const {
    return _imageView;
}

inline const API::Sampler& Texture::getSampler() const {
    return _sampler;
}

