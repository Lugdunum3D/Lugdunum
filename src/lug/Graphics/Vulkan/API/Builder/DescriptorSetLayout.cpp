#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

DescriptorSetLayout::DescriptorSetLayout(const API::Device& device) : _device{device} {}

bool DescriptorSetLayout::build(API::DescriptorSetLayout& descriptorSetLayout, VkResult* returnResult) {
    // Create the descriptorSetLayout creation information for vkCreateDescriptorSetLayout
    const VkDescriptorSetLayoutCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.bindingCount */ static_cast<uint32_t>(_bindings.size()),
        /* createInfo.pBindings */ _bindings.data(),
    };

    // Create the descriptorSetLayout
    VkDescriptorSetLayout vkDescriptorSetLayout{VK_NULL_HANDLE};
    VkResult result = vkCreateDescriptorSetLayout(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkDescriptorSetLayout);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    descriptorSetLayout = API::DescriptorSetLayout(vkDescriptorSetLayout, &_device);

    return true;
}

std::unique_ptr<API::DescriptorSetLayout> DescriptorSetLayout::build(VkResult* returnResult) {
    std::unique_ptr<API::DescriptorSetLayout> descriptorSetLayout = std::make_unique<API::DescriptorSetLayout>();
    return build(*descriptorSetLayout, returnResult) ? std::move(descriptorSetLayout) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
