inline SubBuffer::SubBuffer(const API::Buffer* buffer, uint32_t offset, uint32_t size) : _buffer(buffer), _offset(offset), _size(size) {}

inline const API::Buffer* SubBuffer::getBuffer() const {
    return _buffer;
}

inline uint32_t SubBuffer::getOffset() const {
    return _offset;
}

inline uint32_t SubBuffer::getSize() const {
    return _size;
}

inline size_t SubBuffer::getHash() const {
    return _hash;
}

inline void SubBuffer::setHash(size_t hash) {
    _hash = hash;
}
