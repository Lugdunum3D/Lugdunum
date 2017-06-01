#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>

#include <algorithm>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

PipelineLayout::PipelineLayout(const API::Device& device) : _device{device} {}

bool PipelineLayout::build(API::PipelineLayout& pipelineLayout, VkResult* returnResult) {
    // Build a vector of VkDescriptorSetLayout from the API::DescriptorSetLayout
    std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts(_descriptorSetLayouts.size());
    std::transform(
        begin(_descriptorSetLayouts), end(_descriptorSetLayouts), begin(vkDescriptorSetLayouts),
        [](API::DescriptorSetLayout& descriptorSetLayout){ return static_cast<VkDescriptorSetLayout>(descriptorSetLayout); }
    );

    // Create the pipelineLayout creation information for vkCreatePipelineLayout
    const VkPipelineLayoutCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.setLayoutCount */ static_cast<uint32_t>(vkDescriptorSetLayouts.size()),
        /* createInfo.pSetLayouts */ vkDescriptorSetLayouts.data(),
        /* createInfo.pushConstantRangeCount */ static_cast<uint32_t>(_pushConstants.size()),
        /* createInfo.pPushConstantRanges */ _pushConstants.data(),
    };

    // Create the pipelineLayout
    VkPipelineLayout vkPipelineLayout{VK_NULL_HANDLE};
    VkResult result = vkCreatePipelineLayout(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkPipelineLayout);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    pipelineLayout = API::PipelineLayout(vkPipelineLayout, &_device, std::move(_descriptorSetLayouts));

    return true;
}

std::unique_ptr<API::PipelineLayout> PipelineLayout::build(VkResult* returnResult) {
    std::unique_ptr<API::PipelineLayout> pipelineLayout = std::make_unique<API::PipelineLayout>();
    return build(*pipelineLayout, returnResult) ? std::move(pipelineLayout) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
