#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API DescriptorPool {
public:
    DescriptorPool(VkDescriptorPool descriptorPool = VK_NULL_HANDLE, const Device *device = nullptr);

    DescriptorPool(const DescriptorPool&) = delete;
    // Warning: Don't move DescriptorPool after creating a DescriptorSet
    DescriptorPool(DescriptorPool&& DescriptorPool);

    DescriptorPool& operator=(const DescriptorPool&) = delete;
    // Warning: Don't move DescriptorPool after creating a DescriptorSet
    DescriptorPool& operator=(DescriptorPool&& DescriptorPool);

    ~DescriptorPool();

    operator VkDescriptorPool() const {
        return _descriptorPool;
    }

    std::vector<DescriptorSet> createDescriptorSets(const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);

    void destroy();

private:
    VkDescriptorPool _descriptorPool{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
