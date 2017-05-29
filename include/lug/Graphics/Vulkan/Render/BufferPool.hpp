#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <set>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class DescriptorPool;
class DescriptorSetLayout;
} // API

namespace Render {

class LUG_GRAPHICS_API BufferPool {
    struct Chunk;

public:
    class SubBuffer{
        friend BufferPool;

    public:
        SubBuffer() = default;
        SubBuffer(const API::DescriptorSet* descriptorSet, const API::Buffer* buffer, uint32_t offset, uint32_t size, Chunk* chunk);

        SubBuffer(const SubBuffer&) = default;
        SubBuffer(SubBuffer&& subBuffer) = default;

        SubBuffer& operator=(const SubBuffer&) = default;
        SubBuffer& operator=(SubBuffer&& subBuffer) = default;

        void free();

    public:
        const API::DescriptorSet* descriptorSet;
        const API::Buffer* buffer;
        uint32_t offset;
        uint32_t size;

    private:
        Chunk* chunk;
    };

private:
    struct Chunk {
        API::DeviceMemory bufferMemory;
        API::DescriptorSet descriptorSet;
        API::Buffer buffer;

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
                const API::Device& device,
                const std::set<uint32_t>& queueFamilyIndices,
                const API::DescriptorPool& descriptorPool,
                const API::DescriptorSetLayout* descriptorSetLayout);

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

    // Use a list to avoid the change of adress of a chunk if we add another chunk to the list
    std::list<Chunk> _chunks;

    const API::Device& _device;
    std::set<uint32_t> _queueFamilyIndices;
    const API::DescriptorPool& _descriptorPool;
    const API::DescriptorSetLayout* _descriptorSetLayout;
};

} // Render
} // Vulkan
} // Graphics
} // lug
