#include <lug/Graphics/Vulkan/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

DescriptorSet::DescriptorSet(VkDescriptorSet descriptorSet, const Device* device) : _descriptorSet(descriptorSet), _device(device) {}

DescriptorSet::DescriptorSet(DescriptorSet&& descriptorSet) {
    _descriptorSet = descriptorSet._descriptorSet;
    _device = descriptorSet._device;

    descriptorSet._descriptorSet = VK_NULL_HANDLE;
    descriptorSet._device = nullptr;

}

DescriptorSet& DescriptorSet::operator=(DescriptorSet&& descriptorSet) {
    destroy();

    _descriptorSet = descriptorSet._descriptorSet;
    _device = descriptorSet._device;

    descriptorSet._descriptorSet = VK_NULL_HANDLE;
    descriptorSet._device = nullptr;

    return *this;
}

DescriptorSet::~DescriptorSet() {
    destroy();
}

void DescriptorSet::bind(const PipelineLayout* pipelineLayout,
                            const CommandBuffer* commandBuffer,
                            uint32_t setNb,
                            uint32_t dynamicOffsetsCount,
                            const uint32_t* dynamicOffsets) {
    vkCmdBindDescriptorSets(*commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            *pipelineLayout, setNb,
                            1,
                            &_descriptorSet,
                            dynamicOffsetsCount,
                            dynamicOffsets);
}

void DescriptorSet::update(VkDescriptorType descriptorType, uint32_t dstBinding, const Buffer* buffer, uint32_t offset, uint32_t range) {
    VkDescriptorBufferInfo bufferInfo{
        bufferInfo.buffer = *buffer,
        bufferInfo.offset = offset,
        bufferInfo.range = range,
    };

    VkWriteDescriptorSet write{
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        write.pNext = nullptr,
        write.dstSet = _descriptorSet,
        write.dstBinding = dstBinding,
        write.dstArrayElement = 0,
        write.descriptorCount = 1,
        write.descriptorType = descriptorType,
        write.pImageInfo = nullptr,
        write.pBufferInfo = &bufferInfo,
        write.pTexelBufferView = nullptr
    };

    vkUpdateDescriptorSets(*_device, 1, &write, 0, nullptr);
}

void DescriptorSet::destroy() {
    if (_descriptorSet != VK_NULL_HANDLE) {
        _descriptorSet = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphics
} // lug
