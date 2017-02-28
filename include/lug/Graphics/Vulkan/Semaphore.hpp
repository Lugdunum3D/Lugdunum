#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API Semaphore {
public:
    explicit Semaphore(VkSemaphore semaphore = VK_NULL_HANDLE, const Device* device = nullptr);

    Semaphore(const Semaphore&) = delete;
    Semaphore(Semaphore&& semaphore);

    Semaphore& operator=(const Semaphore&) = delete;
    Semaphore& operator=(Semaphore&& semaphore);

    ~Semaphore();

    explicit operator VkSemaphore() const {
        return _semaphore;
    }

    void destroy();

private:
    VkSemaphore _semaphore{ VK_NULL_HANDLE };
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
