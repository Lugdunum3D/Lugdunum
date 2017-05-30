#include <lug/Graphics/Vulkan/API/DescriptorPool.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

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
