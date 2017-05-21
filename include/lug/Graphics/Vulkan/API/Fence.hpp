#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {
class Fence;
}

class LUG_GRAPHICS_API Fence {
    friend Builder::Fence;

public:
    Fence() = default;

    Fence(const Fence&) = delete;
    Fence(Fence&& fence);

    Fence& operator=(const Fence&) = delete;
    Fence& operator=(Fence&& fence);

    ~Fence();

    explicit operator VkFence() const {
        return _fence;
    }

    VkResult getStatus() const;
    bool reset();
    bool wait() const;

    void destroy();

private:
    explicit Fence(VkFence fence, const Device* device);

private:
    VkFence _fence{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
