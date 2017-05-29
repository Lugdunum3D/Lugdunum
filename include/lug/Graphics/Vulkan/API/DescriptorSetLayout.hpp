#pragma once

#include <memory>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class DescriptorSetLayout;
} // Builder

class Device;

class LUG_GRAPHICS_API DescriptorSetLayout {
    friend class Builder::DescriptorSetLayout;

public:
    DescriptorSetLayout() = default;

    DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout(DescriptorSetLayout&& device);

    DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout& operator=(DescriptorSetLayout&& device);

    ~DescriptorSetLayout();

    explicit operator VkDescriptorSetLayout() const {
        return _descriptorSetLayout;
    }

    /**
     * @brief      Gets the device associated with this DescriptorSetLayout.
     *
     * @return     The device.
     */
    const Device* getDevice() const;

    void destroy();

private:
    explicit DescriptorSetLayout(VkDescriptorSetLayout DescriptorSetLayout, const Device* device);

private:
    VkDescriptorSetLayout _descriptorSetLayout{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

#include <lug/Graphics/Vulkan/API/DescriptorSetLayout.inl>

} // API
} // Vulkan
} // Graphics
} // lug
