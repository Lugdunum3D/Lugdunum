#include <lug/Graphics/Vulkan/BufferPool.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

BufferPool::SubBuffer::SubBuffer(DescriptorSet* descriptorSet, Buffer* buffer, uint32_t offset, uint32_t size, BufferPool::Chunk* chunk) :
                    descriptorSet(descriptorSet), buffer(buffer), offset(offset), size(size), chunk(chunk) {}

BufferPool::BufferPool(uint32_t countPerChunk,
                        uint32_t subBufferSize,
                        const Device* device,
                        const std::vector<uint32_t>& queueFamilyIndices,
                        DescriptorPool* descriptorPool,
                        const DescriptorSetLayout* descriptorSetLayout) :
                _countPerChunk(countPerChunk), _subBufferSize(subBufferSize), _device(device), _queueFamilyIndices(queueFamilyIndices),
                _descriptorPool(descriptorPool), _descriptorSetLayout(descriptorSetLayout) {}

BufferPool::SubBuffer* BufferPool::allocate() {
    // Search free sub-buffer
    {
        for (auto& chunk: _chunks) {
            BufferPool::SubBuffer* subBuffer = chunk.getFreeSubBuffer();
            if (subBuffer) {
                return subBuffer;
            }
        }
    }

    // No free sub-buffer found, allocate new chunk
    uint32_t subBufferSizeAligned = Buffer::getSizeAligned(_device, _subBufferSize);
    _chunks.resize(_chunks.size() + 1);
    BufferPool::Chunk& chunk = _chunks.back();
    {
        // Create buffer
        chunk.size = subBufferSizeAligned * _countPerChunk;
        chunk.buffer = Buffer::create(_device,
                                    (uint32_t)_queueFamilyIndices.size(),
                                    _queueFamilyIndices.data(),
                                    chunk.size,
                                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
        if (!chunk.buffer) {
            return nullptr;
        }

        // Allocate memory for buffer
        const VkMemoryRequirements& bufferRequirements = chunk.buffer->getRequirements();
        uint32_t memoryTypeIndex = DeviceMemory::findMemoryType(_device, bufferRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        chunk.bufferMemory = DeviceMemory::allocate(_device, chunk.size, memoryTypeIndex);
        if (!chunk.bufferMemory) {
            return nullptr;
        }

        // Bind memory to buffer
        chunk.buffer->bindMemory(chunk.bufferMemory.get(), 0);

        // Create and update descriptor set
        std::vector<DescriptorSet> descriptorSets = _descriptorPool->createDescriptorSets({*_descriptorSetLayout});
        if (descriptorSets.size() == 0) {
            return nullptr;
        }
        chunk.descriptorSet = std::move(descriptorSets[0]);
        chunk.descriptorSet.update(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC , 0, chunk.buffer.get(), 0, subBufferSizeAligned);
    }

    // Init new chunk sub-buffers
    {
        chunk.subBuffers.resize(_countPerChunk);
        chunk.subBuffersFree.resize(_countPerChunk);
        uint32_t offset = 0;
        for (uint32_t i = 0; i < _countPerChunk; ++i) {
            SubBuffer subBuffer{&chunk.descriptorSet,
                chunk.buffer.get(),
                offset,
                subBufferSizeAligned,
                &chunk
            };
            chunk.subBuffers[i] = std::move(subBuffer);

            // Warning: Do not resize chunk.subBuffers
            chunk.subBuffersFree[i] = &chunk.subBuffers[i];

            offset += subBufferSizeAligned;
        }
    }

    return allocate();
}

void BufferPool::free(SubBuffer* subBuffer) {
    subBuffer->chunk->subBuffersFree.push_back(subBuffer);
}

} // Vulkan
} // Graphics
} // lug
