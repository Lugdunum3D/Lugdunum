#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API DescriptorSetLayout {
public:
    explicit DescriptorSetLayout(VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE, const Device* device = nullptr);

    DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout(DescriptorSetLayout&& device);

    DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout& operator=(DescriptorSetLayout&& device);

    ~DescriptorSetLayout();

    explicit operator VkDescriptorSetLayout() const {
        return _descriptorSetLayout;
    }

    void destroy();

    static std::unique_ptr<DescriptorSetLayout> create(const Device* device, VkDescriptorSetLayoutBinding layoutBindings[], uint32_t bindingCount);

private:
    VkDescriptorSetLayout _descriptorSetLayout{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
