#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class PipelineLayout;
} // Builder

class Device;

class LUG_GRAPHICS_API PipelineLayout {
    friend class Builder::PipelineLayout;

public:
    PipelineLayout() = default;

    PipelineLayout(const PipelineLayout&) = delete;
    PipelineLayout(PipelineLayout&& device);

    PipelineLayout& operator=(const PipelineLayout&) = delete;
    PipelineLayout& operator=(PipelineLayout&& device);

    ~PipelineLayout();

    explicit operator VkPipelineLayout() const {
        return _pipelineLayout;
    }

    /**
     * @brief      Gets the DescriptorSetLayouts of the PipelineLayout.
     *             The PipelineLayout owns these DescriptorSetLayouts.
     *
     * @return     The DescriptorSetLayouts.
     */
    const std::vector<DescriptorSetLayout>& getDescriptorSetLayouts() const;

    /**
     * @brief      Gets the device assiciated with this PipelineLayout.
     *
     * @return     The device.
     */
    const Device* getDevice() const;

    void destroy();

private:
    explicit PipelineLayout(
        VkPipelineLayout pipelineLayout,
        const Device* device,
        std::vector<DescriptorSetLayout> descriptorSetLayouts
    );

private:
    VkPipelineLayout _pipelineLayout{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    std::vector<DescriptorSetLayout> _descriptorSetLayouts{};
};

#include <lug/Graphics/Vulkan/API/PipelineLayout.inl>

} // API
} // Vulkan
} // Graphics
} // lug
