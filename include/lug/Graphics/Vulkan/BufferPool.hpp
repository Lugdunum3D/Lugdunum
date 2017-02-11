#pragma once

#include <cstdint>
#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/Graphics/Vulkan/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API BufferPool {
struct Chunk;

public:
    class SubBuffer{
        friend BufferPool;

    public:
        SubBuffer() = default;
        SubBuffer(DescriptorSet* descriptorSet, Buffer* buffer, uint32_t offset, uint32_t size, Chunk* chunk);

        SubBuffer(const SubBuffer&) = default;
        SubBuffer(SubBuffer&& subBuffer) = default;

        SubBuffer& operator=(const SubBuffer&) = default;
        SubBuffer& operator=(SubBuffer&& subBuffer) = default;

    public:
        DescriptorSet* descriptorSet;
        Buffer* buffer;
        uint32_t offset;
        uint32_t size;

    private:
        Chunk* chunk;
    };

private:
    struct Chunk {
        std::unique_ptr<DeviceMemory> bufferMemory;
        DescriptorSet descriptorSet;
        std::unique_ptr<Buffer> buffer;
        uint32_t size;
        // Warning: Do not resize chunk.subBuffers
        std::vector<SubBuffer> subBuffers;
        std::vector<SubBuffer*> subBuffersFree;

        SubBuffer* getFreeSubBuffer() {
            if (subBuffersFree.size() == 0) {
                return nullptr;
            }
            BufferPool::SubBuffer* subBuffer = subBuffersFree.back();
            subBuffersFree.pop_back();
            return subBuffer;
        };
    };

public:
    BufferPool(uint32_t countPerChunk,
                uint32_t subBufferSize,
                const Device* device,
                const std::vector<uint32_t>& queueFamilyIndices,
                DescriptorPool* descriptorPool,
                const DescriptorSetLayout* descriptorSetLayout);

    BufferPool(const BufferPool&) = delete;
    BufferPool(BufferPool&&) = default;

    BufferPool& operator=(const BufferPool&) = delete;
    BufferPool& operator=(BufferPool&&) = default;

    ~BufferPool() = default;

    SubBuffer* allocate();
    void free(SubBuffer* subBuffer);

private:
    uint32_t _countPerChunk;
    uint32_t _subBufferSize;
    std::vector<Chunk> _chunks;

    const Device* _device;
    std::vector<uint32_t> _queueFamilyIndices;
    DescriptorPool* _descriptorPool;
    const DescriptorSetLayout* _descriptorSetLayout;
};

} // Vulkan
} // Graphics
} // lug
