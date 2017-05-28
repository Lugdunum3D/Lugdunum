#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class DescriptorSet;
} // Builder

class Buffer;
class Device;

class LUG_GRAPHICS_API DescriptorSet {
friend Builder::DescriptorSet;

public:
    DescriptorSet() = default;

    DescriptorSet(const DescriptorSet&) = delete;
    DescriptorSet(DescriptorSet&& DescriptorSet);

    DescriptorSet& operator=(const DescriptorSet&) = delete;
    DescriptorSet& operator=(DescriptorSet&& DescriptorSet);

    ~DescriptorSet();

    explicit operator VkDescriptorSet() const {
        return _descriptorSet;
    }

    /**
     * @brief      Update the descriptor set
     *
     * @param[in]  descriptorType  The descriptor type
     * @param[in]  dstBinding      The destination binding
     * @param[in]  buffer          The buffer to bind to the descriptor set
     * @param[in]  offset          The offset in the buffer
     * @param[in]  range           The range of the set
     */
    void update(VkDescriptorType descriptorType, uint32_t dstBinding, const Buffer* buffer, VkDeviceSize offset, VkDeviceSize range) const;

    void destroy();

private:
    explicit DescriptorSet(VkDescriptorSet descriptorSet, const Device* device);

private:
    VkDescriptorSet _descriptorSet{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
