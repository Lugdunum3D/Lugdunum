#include <lug/Graphics/Vulkan/API/Fence.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Fence::Fence(VkFence fence, const Device* device) : _fence(fence), _device(device) {}

Fence::Fence(Fence&& fence) {
    _fence = fence._fence;
    _device = fence._device;
    fence._fence = VK_NULL_HANDLE;
    fence._device = nullptr;
}

Fence& Fence::operator=(Fence&& fence) {
    destroy();

    _fence = fence._fence;
    _device = fence._device;
    fence._fence = VK_NULL_HANDLE;
    fence._device = nullptr;

    return *this;
}

Fence::~Fence() {
    destroy();
}

VkResult Fence::getStatus() const {
    return vkGetFenceStatus(static_cast<VkDevice>(*_device), _fence);
}

bool Fence::reset() const {
    VkResult result = vkResetFences(static_cast<VkDevice>(*_device), 1, &_fence);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("Fence::reset: Can't reset fence: {}", result);
        return false;
    }

    return true;
}

bool Fence::wait() const {
    VkResult result = vkWaitForFences(static_cast<VkDevice>(*_device), 1, &_fence, VK_TRUE, UINT64_MAX);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("Fence::wait: Can't wait for fence: {}", result);
        return false;
    }

    return true;
}

void Fence::destroy() {
    if (_fence != VK_NULL_HANDLE) {
        vkDestroyFence(static_cast<VkDevice>(*_device), _fence, nullptr);
        _fence = VK_NULL_HANDLE;
    }
    _device = nullptr;
}

} // API
} // Vulkan
} // Graphics
} // lug
