#pragma once

#include <cstdint>
#include <cstdlib>

#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class Buffer;
} // API

namespace Render {
namespace BufferPool {

template <size_t subBufferPerChunk, size_t subBufferSize>
class BufferPool;

template <size_t subBufferPerChunk, size_t subBufferSize>
class Chunk;

class LUG_GRAPHICS_API SubBuffer {
    template <size_t subBufferPerChunk, size_t subBufferSize>
    friend class BufferPool;

    template <size_t subBufferPerChunk, size_t subBufferSize>
    friend class Chunk;

public:
    SubBuffer() = default;
    SubBuffer(const API::Buffer* buffer, uint32_t offset, uint32_t size);

    SubBuffer(const SubBuffer&) = delete;
    SubBuffer(SubBuffer&&) = default;

    SubBuffer& operator=(const SubBuffer&) = delete;
    SubBuffer& operator=(SubBuffer&&) = default;

    ~SubBuffer() = default;

    const API::Buffer* getBuffer() const;
    uint32_t getOffset() const;
    uint32_t getSize() const;

    size_t getHash() const;
    void setHash(size_t hash);

private:
    const API::Buffer* _buffer{nullptr};
    uint32_t _offset{0};
    uint32_t _size{0};

    size_t _hash{0};
    uint32_t _referenceCount{0};
};

#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.inl>

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
