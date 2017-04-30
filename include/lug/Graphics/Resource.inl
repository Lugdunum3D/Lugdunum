inline Resource::Type Resource::getType() const {
    return static_cast<Resource::Type>(_handle.type);
}

inline Resource::Handle Resource::getHandle() const {
    return _handle;
}
