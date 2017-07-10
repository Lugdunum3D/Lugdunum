template <size_t subBufferPerChunk, size_t subBufferSize>
inline bool Chunk<subBufferPerChunk, subBufferSize>::init(Renderer& renderer, std::set<uint32_t> queueFamilyIndices) {
    // Allocate the memory
    const auto& alignment = renderer.getDevice().getPhysicalDeviceInfo()->properties.limits.minUniformBufferOffsetAlignment;
    VkDeviceSize subBufferSizeAligned = subBufferSize;

    if (subBufferSizeAligned % alignment) {
        subBufferSizeAligned += alignment - subBufferSizeAligned % alignment;
    }

    // Create buffer
    {
        API::Builder::Buffer bufferBuilder(renderer.getDevice());

        bufferBuilder.setQueueFamilyIndices(queueFamilyIndices);
        bufferBuilder.setSize(subBufferSizeAligned * (subBufferPerChunk - 1) + subBufferSize);
        bufferBuilder.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

        VkResult result{VK_SUCCESS};
        if (!bufferBuilder.build(_buffer, &result)) {
            LUG_LOG.error("BufferPool::Chunk: Can't create buffer: {}", result);
            return false;
        }
    }

    // Create buffer memory
    {
        API::Builder::DeviceMemory deviceMemoryBuilder(renderer.getDevice());
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (!deviceMemoryBuilder.addBuffer(_buffer)) {
            LUG_LOG.error("BufferPool::Chunk: Can't add buffer to device memory");
            return false;
        }

        VkResult result{VK_SUCCESS};
        if (!deviceMemoryBuilder.build(_bufferMemory, &result)) {
            LUG_LOG.error("BufferPool::Chunk: Can't create device memory: {}", result);
            return false;
        }
    }

    // Init subBuffers
    for (uint32_t i = 0; i < subBufferPerChunk; ++i) {
        _subBuffers[i] = SubBuffer(
            &_buffer,
            static_cast<uint32_t>(subBufferSizeAligned * i),
            static_cast<uint32_t>(subBufferSize)
        );
    }

    return true;
}

template <size_t subBufferPerChunk, size_t subBufferSize>
inline SubBuffer* Chunk<subBufferPerChunk, subBufferSize>::getFreeSubBuffer() {
    for (auto& subBuffer : _subBuffers) {
        if (subBuffer._referenceCount == 0) {
            return &subBuffer;
        }
    }

    return nullptr;
}
