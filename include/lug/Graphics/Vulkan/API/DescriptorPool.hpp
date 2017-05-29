#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class DescriptorPool;
} // Builder

class Device;

class LUG_GRAPHICS_API DescriptorPool {
    friend class Builder::DescriptorPool;

public:
    DescriptorPool() = default;

    DescriptorPool(const DescriptorPool&) = delete;
    // Warning: Don't move DescriptorPool after creating a DescriptorSet
    DescriptorPool(DescriptorPool&& DescriptorPool);

    DescriptorPool& operator=(const DescriptorPool&) = delete;
    // Warning: Don't move DescriptorPool after creating a DescriptorSet
    DescriptorPool& operator=(DescriptorPool&& DescriptorPool);

    ~DescriptorPool();

    explicit operator VkDescriptorPool() const {
        return _descriptorPool;
    }

    void destroy();

private:
    explicit DescriptorPool(VkDescriptorPool descriptorPool, const Device *device);

private:
    VkDescriptorPool _descriptorPool{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
