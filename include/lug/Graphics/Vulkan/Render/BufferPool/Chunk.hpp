#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool/SubBuffer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace BufferPool {

template <size_t subBufferPerChunk, size_t subBufferSize>
class LUG_GRAPHICS_API Chunk {
public:
    Chunk() = default;

    Chunk(const Chunk&) = delete;
    Chunk(Chunk&&) = delete;

    Chunk& operator=(const Chunk&) = delete;
    Chunk& operator=(Chunk&&) = delete;

    ~Chunk() = default;

    bool init(Renderer& renderer, std::set<uint32_t> queueFamilyIndices);

    SubBuffer* getFreeSubBuffer();

private:
    API::DeviceMemory _bufferMemory{};
    API::Buffer _buffer{};

    std::array<SubBuffer, subBufferPerChunk> _subBuffers{};
};

#include <lug/Graphics/Vulkan/Render/BufferPool/Chunk.inl>

} // BufferPool
} // Render
} // Vulkan
} // Graphics
} // lug
