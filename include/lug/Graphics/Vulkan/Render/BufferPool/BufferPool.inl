template <size_t subBufferPerChunk, size_t subBufferSize>
inline BufferPool<subBufferPerChunk, subBufferSize>::BufferPool(Renderer& renderer, std::set<uint32_t> queueFamilyIndices)
    : _renderer(renderer), _queueFamilyIndices(queueFamilyIndices)
{}

template <size_t subBufferPerChunk, size_t subBufferSize>
inline std::tuple<bool, const SubBuffer*> BufferPool<subBufferPerChunk, subBufferSize>::allocate(size_t hash, bool dirty) {
    auto it = _subBuffersInUse.find(hash);
    if (it == _subBuffersInUse.end() || dirty) {
        SubBuffer* subBuffer = allocateNewBuffer();

        if (!subBuffer) {
            return std::make_tuple(false, nullptr);
        }

        subBuffer->setHash(hash);
        _subBuffersInUse[hash] = subBuffer;

        return std::make_tuple(true, subBuffer);
    }

    it->second->_referenceCount += 1;
    return std::make_tuple(false, it->second);
}

template <size_t subBufferPerChunk, size_t subBufferSize>
inline void BufferPool<subBufferPerChunk, subBufferSize>::free(const SubBuffer* buffer) {
    if (!buffer) {
        return;
    }

    const_cast<SubBuffer*>(buffer)->_referenceCount -= 1;

    const auto it = _subBuffersInUse.find(buffer->getHash());
    if (buffer->_referenceCount == 0 && it != _subBuffersInUse.end() && it->second == buffer) {
        _subBuffersInUse.erase(buffer->getHash());
    }
}

template <size_t subBufferPerChunk, size_t subBufferSize>
inline SubBuffer* BufferPool<subBufferPerChunk, subBufferSize>::allocateNewBuffer() {
    for (auto& chunk : _chunks) {
        SubBuffer* subBuffer = chunk.getFreeSubBuffer();

        if (subBuffer) {
            subBuffer->_referenceCount += 1;
            return subBuffer;
        }
    }

    // Reallocate a new chunk
    {
        _chunks.emplace_back();

        if (_chunks.back().init(_renderer, _queueFamilyIndices)) {
            return allocateNewBuffer();
        }

        _chunks.pop_back();
    }

    return nullptr;
}
