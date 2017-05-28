#include <lug/Graphics/Vulkan/API/Builder/DescriptorSet.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

DescriptorSet::DescriptorSet(const API::Device& device, const API::DescriptorPool& descriptorPool) : _device{device}, _descriptorPool(descriptorPool) {}

bool DescriptorSet::build(API::DescriptorSet& instance, VkResult* returnResult) {
    std::vector<API::DescriptorSet> descriptorSets(1);
    if (!build(descriptorSets, returnResult)) {
        return false;
    }
    instance = std::move(descriptorSets[0]);
    return true;
}

std::unique_ptr<API::DescriptorSet> DescriptorSet::build(VkResult* returnResult) {
    std::vector<API::DescriptorSet> descriptorSets(1);
    return build(descriptorSets, returnResult) ? std::make_unique<API::DescriptorSet>(std::move(descriptorSets[0])) : nullptr;
}

bool DescriptorSet::build(std::vector<API::DescriptorSet>& descriptorSets, VkResult* returnResult) {
    // Create the descriptorSet creation information for vkCreateDescriptorSet
    const VkDescriptorSetAllocateInfo allocateInfo{
        /* allocateInfo.sType */ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        /* allocateInfo.pNext */ nullptr,
        /* allocateInfo.descriptorPool */ static_cast<VkDescriptorPool>(_descriptorPool),
        /* allocateInfo.descriptorSetCount */ static_cast<uint32_t>(descriptorSets.size()),
        /* allocateInfo.pSetLayouts */ _descriptorSetLayouts.data(),
    };

    std::vector<VkDescriptorSet> vkDescriptorSets(descriptorSets.size());

    // Create the descriptorSets
    VkResult result = vkAllocateDescriptorSets(static_cast<VkDevice>(_device), &allocateInfo, vkDescriptorSets.data());

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    for (uint32_t i = 0; i < descriptorSets.size(); ++i) {
        descriptorSets[i] = API::DescriptorSet(vkDescriptorSets[i], &_device);
    }

    return true;
}

std::vector<std::unique_ptr<API::DescriptorSet>> DescriptorSet::build(uint32_t count, VkResult* returnResult) {
    // Output vector
    std::vector<API::DescriptorSet> descriptorSets(count);
    std::vector<std::unique_ptr<API::DescriptorSet>> outDescriptorSets(count);

    // Build the command buffers
    if (!build(descriptorSets, returnResult)) {
        return {};
    }

    // Initialize the unique_ptr array
    for (uint32_t i = 0; i < count; ++i) {
        outDescriptorSets[i] = std::make_unique<API::DescriptorSet>(std::move(descriptorSets[i]));
    }

    return outDescriptorSets;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
