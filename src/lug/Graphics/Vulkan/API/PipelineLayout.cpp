#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

PipelineLayout::PipelineLayout(
    std::vector<DescriptorSetLayout>& descriptorSetLayouts,
    VkPipelineLayout pipelineLayout,
    const Device* device) :
    _pipelineLayout(pipelineLayout), _device(device), _descriptorSetLayouts(std::move(descriptorSetLayouts)) {}

PipelineLayout::PipelineLayout(PipelineLayout&& pipelineLayout) {
    _pipelineLayout = pipelineLayout._pipelineLayout;
    _device = pipelineLayout._device;
    _descriptorSetLayouts = std::move(pipelineLayout._descriptorSetLayouts);

    pipelineLayout._pipelineLayout = VK_NULL_HANDLE;
    pipelineLayout._device = nullptr;
    pipelineLayout._descriptorSetLayouts.resize(0);
}

PipelineLayout& PipelineLayout::operator=(PipelineLayout&& pipelineLayout) {
    destroy();

    _pipelineLayout = pipelineLayout._pipelineLayout;
    _device = pipelineLayout._device;
    _descriptorSetLayouts = std::move(pipelineLayout._descriptorSetLayouts);

    pipelineLayout._pipelineLayout = VK_NULL_HANDLE;
    pipelineLayout._device = nullptr;
    pipelineLayout._descriptorSetLayouts.resize(0);

    return *this;
}

PipelineLayout::~PipelineLayout() {
    destroy();
}

void PipelineLayout::destroy() {
    if (_pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(static_cast<VkDevice>(*_device), _pipelineLayout, nullptr);
        _pipelineLayout = VK_NULL_HANDLE;
    }

    _descriptorSetLayouts.clear();
}

std::unique_ptr<PipelineLayout> PipelineLayout::create(const Device* device) {
    std::vector<DescriptorSetLayout> descriptorSetLayouts(2);
    VkResult result{VK_SUCCESS};
    Builder::DescriptorSetLayout descriptorSetLayoutBuilderInstance(*device);


    // Bindings set 0
    {
        // Camera uniform buffer
        VkDescriptorSetLayoutBinding binding{
            binding.binding = 0,
            binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
            binding.descriptorCount = 1,
            binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            binding.pImmutableSamplers = nullptr // Only used for descriptorType VK_DESCRIPTOR_TYPE_SAMPLER or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        };

        descriptorSetLayoutBuilderInstance.setBindings({binding});
        if (!descriptorSetLayoutBuilderInstance.build(descriptorSetLayouts[0], &result)) {
            LUG_LOG.error("PipelineLayout::create: Can't create pipeline descriptor sets layout 0: {}", result);
            return nullptr;
        }
    }

    // Bindings set 1
    {
        // Light uniform buffer
        VkDescriptorSetLayoutBinding binding{
            binding.binding = 0,
            binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
            binding.descriptorCount = 1,
            binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            binding.pImmutableSamplers = nullptr // Only used for descriptorType VK_DESCRIPTOR_TYPE_SAMPLER or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        };

        descriptorSetLayoutBuilderInstance.setBindings({binding});
        if (!descriptorSetLayoutBuilderInstance.build(descriptorSetLayouts[1], &result)) {
            LUG_LOG.error("PipelineLayout::create: Can't create pipeline descriptor sets layout 1: {}", result);
            return nullptr;
        }
    }

    VkDescriptorSetLayout vkDescriptorSetLayouts[] = {
        static_cast<VkDescriptorSetLayout>(descriptorSetLayouts[0]),
        static_cast<VkDescriptorSetLayout>(descriptorSetLayouts[1])
    };

    VkPushConstantRange pushConstants[] = {
        // Model transformation
        {
            pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            pushConstants[0].offset = 0,
            pushConstants[0].size = sizeof(Math::Mat4x4f)
        }
    };

    VkPipelineLayoutCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.setLayoutCount = 2,
        createInfo.pSetLayouts = vkDescriptorSetLayouts,
        createInfo.pushConstantRangeCount = 1,
        createInfo.pPushConstantRanges = pushConstants
    };

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    result = vkCreatePipelineLayout(static_cast<VkDevice>(*device), &createInfo, nullptr, &pipelineLayout);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create pipeline layout: {}", result);
        return nullptr;
    }

    return std::unique_ptr<PipelineLayout>(new PipelineLayout(descriptorSetLayouts, pipelineLayout, device));
}

} // API
} // Vulkan
} // Graphics
} // lug
