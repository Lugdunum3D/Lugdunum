#include <lug/Graphics/Vulkan/Fence.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

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

VkResult Fence::getStatus() const
{
    return vkGetFenceStatus(*_device, _fence);
}

bool Fence::reset()
{
    VkResult result = vkResetFences(*_device, 1, &_fence);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererWindow: Can't initialize surface: {}", result);
        return false;
    }

    return true;
}

bool Fence::wait() const
{
    VkResult result = vkWaitForFences(*_device, 1, &_fence, VK_TRUE, UINT64_MAX);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererWindow: Can't initialize surface: {}", result);
        return false;
    }

    return true;
}

void Fence::destroy() {
    if (_fence != VK_NULL_HANDLE) {
        vkDestroyFence(*_device, _fence, nullptr);
        _fence = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphics
} // lug
