#pragma once

#include <list>
#include <map>
#include <set>

#include <lug/Graphics/Vulkan/Render/BufferPool/Chunk.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

namespace Render {
namespace BufferPool {

template <size_t subBufferPerChunk, size_t subBufferSize>
class LUG_GRAPHICS_API BufferPool {
public:
    BufferPool(Renderer& renderer, std::set<uint32_t> queueFamilyIndices);

    BufferPool(const BufferPool&) = delete;
    BufferPool(BufferPool&&) = delete;

    BufferPool& operator=(const BufferPool&) = delete;
    BufferPool& operator=(BufferPool&&) = delete;

    ~BufferPool() = default;

    std::tuple<bool, const SubBuffer*> allocate(size_t hash, bool dirty);
    void free(const SubBuffer* buffer);

private:
    SubBuffer* allocateNewBuffer();

protected:
    Renderer& _renderer;
    std::set<uint32_t> _queueFamilyIndices{};

    std::list<Chunk<subBufferPerChunk, subBufferSize>> _chunks{};
    std::map<size_t, SubBuffer*> _subBuffersInUse{};
};

#include <lug/Graphics/Vulkan/Render/BufferPool/BufferPool.inl>

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
