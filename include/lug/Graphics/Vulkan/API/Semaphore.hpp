#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Semaphore;
} // Builder

class Device;

class LUG_GRAPHICS_API Semaphore {
    friend class Builder::Semaphore;

public:
    Semaphore() = default;

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
    explicit Semaphore(VkSemaphore semaphore, const Device* device);

private:
    VkSemaphore _semaphore{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
