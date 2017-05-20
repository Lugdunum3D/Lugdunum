#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

class LUG_GRAPHICS_API PipelineLayout {
public:
    explicit PipelineLayout(std::vector<DescriptorSetLayout>& descriptorSetLayouts,
                            VkPipelineLayout pipelineLayout = VK_NULL_HANDLE,
                            const Device* device = nullptr);

    PipelineLayout(const PipelineLayout&) = delete;
    PipelineLayout(PipelineLayout&& device);

    PipelineLayout& operator=(const PipelineLayout&) = delete;
    PipelineLayout& operator=(PipelineLayout&& device);

    ~PipelineLayout();

    explicit operator VkPipelineLayout() const {
        return _pipelineLayout;
    }

    const std::vector<DescriptorSetLayout>& getDescriptorSetLayouts() {
        return _descriptorSetLayouts;
    }

    void destroy();

    static std::unique_ptr<PipelineLayout> create(const Device* device);

private:
    VkPipelineLayout _pipelineLayout{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::vector<DescriptorSetLayout> _descriptorSetLayouts;
};

} // API
} // Vulkan
} // Graphics
} // lug
