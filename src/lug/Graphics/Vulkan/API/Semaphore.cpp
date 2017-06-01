#include <lug/Graphics/Vulkan/API/Semaphore.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Semaphore::Semaphore(VkSemaphore semaphore, const Device* device) : _semaphore(semaphore), _device(device) {}

Semaphore::Semaphore(Semaphore&& semaphore) {
    _semaphore = semaphore._semaphore;
    _device = semaphore._device;
    semaphore._semaphore = VK_NULL_HANDLE;
    semaphore._device = nullptr;
}

Semaphore& Semaphore::operator=(Semaphore&& semaphore) {
    destroy();

    _semaphore = semaphore._semaphore;
    _device = semaphore._device;
    semaphore._semaphore = VK_NULL_HANDLE;
    semaphore._device = nullptr;

    return *this;
}

Semaphore::~Semaphore() {
    destroy();
}

void Semaphore::destroy() {
    if (_semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(static_cast<VkDevice>(*_device), _semaphore, nullptr);
        _semaphore = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
