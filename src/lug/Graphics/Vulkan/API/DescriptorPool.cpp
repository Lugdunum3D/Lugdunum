#include <lug/Graphics/Vulkan/API/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

DescriptorPool::DescriptorPool(VkDescriptorPool descriptorPool, const Device* device) : _descriptorPool(descriptorPool), _device(device) {}

DescriptorPool::DescriptorPool(DescriptorPool&& descriptorPool) {
    _descriptorPool = descriptorPool._descriptorPool;
    _device = descriptorPool._device;

    descriptorPool._descriptorPool = VK_NULL_HANDLE;
    descriptorPool._device = nullptr;
}

DescriptorPool& DescriptorPool::operator=(DescriptorPool&& descriptorPool) {
    destroy();

    _descriptorPool = descriptorPool._descriptorPool;
    _device = descriptorPool._device;

    descriptorPool._descriptorPool = VK_NULL_HANDLE;
    descriptorPool._device = nullptr;

    return *this;
}

DescriptorPool::~DescriptorPool() {
    destroy();
}

std::vector<DescriptorSet> DescriptorPool::createDescriptorSets(const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts) {
    std::uint32_t count = (uint32_t)descriptorSetLayouts.size();

    VkDescriptorSetAllocateInfo allocateInfo{
        allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        allocateInfo.pNext = nullptr,
        allocateInfo.descriptorPool = _descriptorPool,
        allocateInfo.descriptorSetCount = count,
        allocateInfo.pSetLayouts = descriptorSetLayouts.data()
    };

    std::vector<VkDescriptorSet> descriptorSets(count);
    VkResult result = vkAllocateDescriptorSets(static_cast<VkDevice>(*_device), &allocateInfo, descriptorSets.data());

    // TODO: Handle VK_ERROR_FRAGMENTED_POOL and negative values to create a new descriptor pool
    if (result != VK_SUCCESS) {
        LUG_LOG.error("DescriptorPool: Can't allocate descriptor sets: {}", result);
        return {};
    }

    std::vector<DescriptorSet> descriptorSetsReturn(count);

    for (uint32_t i = 0; i < count; ++i) {
        descriptorSetsReturn[i] = DescriptorSet(descriptorSets[i], _device);
    }

    return descriptorSetsReturn;
}

void DescriptorPool::destroy() {
    if (_descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(static_cast<VkDevice>(*_device), _descriptorPool, nullptr);
        _descriptorPool = VK_NULL_HANDLE;
    }

    _device = nullptr;
}

} // API
} // Vulkan
} // Graphics
} // lug
