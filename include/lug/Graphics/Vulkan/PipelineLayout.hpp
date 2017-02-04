#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API PipelineLayout {
public:
    explicit PipelineLayout(std::vector<std::unique_ptr<DescriptorSetLayout> >& descriptorSetLayouts,
                            VkPipelineLayout pipelineLayout = VK_NULL_HANDLE,
                            const Device* device = nullptr);

    PipelineLayout(const PipelineLayout&) = delete;
    PipelineLayout(PipelineLayout&& device);

    PipelineLayout& operator=(const PipelineLayout&) = delete;
    PipelineLayout& operator=(PipelineLayout&& device);

    ~PipelineLayout();

    operator VkPipelineLayout() const {
        return _pipelineLayout;
    }

    const std::vector<std::unique_ptr<DescriptorSetLayout> >& getDescriptorSetLayouts() {
        return _descriptorSetLayouts;
    }

    void destroy();

    static std::unique_ptr<PipelineLayout> create(const Device* device);

private:
    VkPipelineLayout _pipelineLayout{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::vector<std::unique_ptr<DescriptorSetLayout> > _descriptorSetLayouts;
};

} // Vulkan
} // Graphics
} // lug
