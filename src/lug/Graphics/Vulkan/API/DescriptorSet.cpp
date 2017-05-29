#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

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

void DescriptorSet::updateBuffers(
    uint32_t dstBinding,
    uint32_t dstArrayElement,
    VkDescriptorType descriptorType,
    const std::vector<VkDescriptorBufferInfo>& bufferInfos
) const {
    VkWriteDescriptorSet write{
        /* write.sType */ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        /* write.pNext */ nullptr,
        /* write.dstSet */ _descriptorSet,
        /* write.dstBinding */ dstBinding,
        /* write.dstArrayElement */ dstArrayElement,
        /* write.descriptorCount */ static_cast<uint32_t>(bufferInfos.size()),
        /* write.descriptorType */ descriptorType,
        /* write.pImageInfo */ nullptr,
        /* write.pBufferInfo */ bufferInfos.data(),
        /* write.pTexelBufferView */ nullptr
    };

    vkUpdateDescriptorSets(static_cast<VkDevice>(*_device), 1, &write, 0, nullptr);
}

void DescriptorSet::updateImages(
    uint32_t dstBinding,
    uint32_t dstArrayElement,
    VkDescriptorType descriptorType,
    const std::vector<VkDescriptorImageInfo>& imageInfos
) const {
    VkWriteDescriptorSet write{
        /* write.sType */ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        /* write.pNext */ nullptr,
        /* write.dstSet */ _descriptorSet,
        /* write.dstBinding */ dstBinding,
        /* write.dstArrayElement */ dstArrayElement,
        /* write.descriptorCount */ static_cast<uint32_t>(imageInfos.size()),
        /* write.descriptorType */ descriptorType,
        /* write.pImageInfo */ imageInfos.data(),
        /* write.pBufferInfo */ nullptr,
        /* write.pTexelBufferView */ nullptr
    };

    vkUpdateDescriptorSets(static_cast<VkDevice>(*_device), 1, &write, 0, nullptr);
}

void DescriptorSet::destroy() {
    if (_descriptorSet != VK_NULL_HANDLE) {
        _descriptorSet = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
