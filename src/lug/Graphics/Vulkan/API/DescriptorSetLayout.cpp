#include <lug/Graphics/Vulkan/API/DescriptorSetLayout.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

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
        vkDestroyDescriptorSetLayout(static_cast<VkDevice>(*_device), _descriptorSetLayout, nullptr);
        _descriptorSetLayout = VK_NULL_HANDLE;
    }
    _device = nullptr;
}

} // API
} // Vulkan
} // Graphics
} // lug
