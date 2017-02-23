#include <lug/Math/Matrix.hpp>
#include <lug/Graphics/Vulkan/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

DescriptorSetLayout::DescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const Device* device) : _descriptorSetLayout(descriptorSetLayout), _device(device) {}

DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& descriptorSetLayout) {
    _descriptorSetLayout = descriptorSetLayout._descriptorSetLayout;
    _device = descriptorSetLayout._device;
    descriptorSetLayout._descriptorSetLayout = VK_NULL_HANDLE;
    descriptorSetLayout._device = nullptr;
}

DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout&& descriptorSetLayout) {
    destroy();

    _descriptorSetLayout = descriptorSetLayout._descriptorSetLayout;
    _device = descriptorSetLayout._device;
    descriptorSetLayout._descriptorSetLayout = VK_NULL_HANDLE;
    descriptorSetLayout._device = nullptr;

    return *this;
}

DescriptorSetLayout::~DescriptorSetLayout() {
    destroy();
}

void DescriptorSetLayout::destroy() {
    if (_descriptorSetLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(*_device, _descriptorSetLayout, nullptr);
        _descriptorSetLayout = VK_NULL_HANDLE;
    }
}

std::unique_ptr<DescriptorSetLayout> DescriptorSetLayout::create(const Device* device, VkDescriptorSetLayoutBinding layoutBindings[], uint32_t bindingCount) {
    VkDescriptorSetLayoutCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.bindingCount = bindingCount,
        createInfo.pBindings = layoutBindings
    };

    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkResult result = vkCreateDescriptorSetLayout(*device, &createInfo, nullptr, &descriptorSetLayout);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create descriptor set layout: {}", result);
        return nullptr;
    }

    return std::unique_ptr<DescriptorSetLayout>(new DescriptorSetLayout(descriptorSetLayout, device));
}

} // Vulkan
} // Graphics
} // lug
