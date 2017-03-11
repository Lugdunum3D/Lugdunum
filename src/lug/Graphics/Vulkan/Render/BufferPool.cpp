#include <lug/Graphics/Vulkan/Render/BufferPool.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

BufferPool::SubBuffer::SubBuffer(API::DescriptorSet* descriptorSet, API::Buffer* buffer, uint32_t offset, uint32_t size, BufferPool::Chunk* chunk) :
                    descriptorSet(descriptorSet), buffer(buffer), offset(offset), size(size), chunk(chunk) {}

BufferPool::BufferPool(uint32_t countPerChunk,
                        uint32_t subBufferSize,
                        const API::Device* device,
                        const std::vector<uint32_t>& queueFamilyIndices,
                        API::DescriptorPool* descriptorPool,
                        const API::DescriptorSetLayout* descriptorSetLayout) :
                _countPerChunk(countPerChunk), _subBufferSize(subBufferSize), _device(device), _queueFamilyIndices(queueFamilyIndices),
                _descriptorPool(descriptorPool), _descriptorSetLayout(descriptorSetLayout) {}

void BufferPool::SubBuffer::free() {
    chunk->subBuffersFree.push_back(this);
}

BufferPool::SubBuffer* BufferPool::allocate() {
    // Search free sub-buffer
    {
        for (auto& chunk: _chunks) {
            BufferPool::SubBuffer* subBuffer = chunk->getFreeSubBuffer();
            if (subBuffer) {
                return subBuffer;
            }
        }
    }

    // No free sub-buffer found, allocate new chunk
    uint32_t subBufferSizeAligned = API::Buffer::getSizeAligned(_device, _subBufferSize);
    std::unique_ptr<BufferPool::Chunk> chunk = std::make_unique<BufferPool::Chunk>();
    {
        // Create buffer
        chunk->size = subBufferSizeAligned * _countPerChunk;
        chunk->buffer = API::Buffer::create(_device,
                                    (uint32_t)_queueFamilyIndices.size(),
                                    _queueFamilyIndices.data(),
                                    chunk->size,
                                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
        if (!chunk->buffer) {
            return nullptr;
        }

        // Allocate memory for buffer
        const VkMemoryRequirements& bufferRequirements = chunk->buffer->getRequirements();
        uint32_t memoryTypeIndex = API::DeviceMemory::findMemoryType(_device, bufferRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        chunk->bufferMemory = API::DeviceMemory::allocate(_device, chunk->size, memoryTypeIndex);
        if (!chunk->bufferMemory) {
            return nullptr;
        }

        // Bind memory to buffer
        chunk->buffer->bindMemory(chunk->bufferMemory.get(), 0);

        // Create and update descriptor set
        std::vector<API::DescriptorSet> descriptorSets = _descriptorPool->createDescriptorSets({static_cast<VkDescriptorSetLayout>(*_descriptorSetLayout)});
        if (descriptorSets.size() == 0) {
            return nullptr;
        }
        chunk->descriptorSet = std::move(descriptorSets[0]);
        chunk->descriptorSet.update(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC , 0, chunk->buffer.get(), 0, subBufferSizeAligned);
    }

    // Init new chunk sub-buffers
    {
        chunk->subBuffers.resize(_countPerChunk);
        chunk->subBuffersFree.resize(_countPerChunk);
        uint32_t offset = 0;
        for (uint32_t i = 0; i < _countPerChunk; ++i) {
            SubBuffer subBuffer{&chunk->descriptorSet,
                chunk->buffer.get(),
                offset,
                subBufferSizeAligned,
                chunk.get()
            };
            chunk->subBuffers[i] = std::move(subBuffer);

            // Warning: Do not resize chunk->subBuffers
            chunk->subBuffersFree[i] = &chunk->subBuffers[i];

            offset += subBufferSizeAligned;
        }
    }

    _chunks.push_back(std::move(chunk));

    return allocate();
}

void BufferPool::free(SubBuffer* subBuffer) {
    subBuffer->free();
}

} // Render
} // Vulkan
} // Graphics
} // lug
