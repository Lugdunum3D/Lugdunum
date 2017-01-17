#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Fence {
public:
    Fence(VkFence fence = VK_NULL_HANDLE, const Device* device = nullptr);

    Fence(const Fence&) = delete;
    Fence(Fence&& fence);

    Fence& operator=(const Fence&) = delete;
    Fence& operator=(Fence&& fence);

    ~Fence();

    operator VkFence() const {
        return _fence;
    }

    VkResult getStatus() const;
    bool reset();
    bool wait() const;

    void destroy();

private:
    VkFence _fence{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
