#include <lug/Math/Matrix.hpp>
#include <lug/Graphics/Vulkan/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

PipelineLayout::PipelineLayout(std::vector<std::unique_ptr<DescriptorSetLayout> >& descriptorSetLayouts,
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
        vkDestroyPipelineLayout(*_device, _pipelineLayout, nullptr);
        _pipelineLayout = VK_NULL_HANDLE;
    }
}

std::unique_ptr<PipelineLayout> PipelineLayout::create(const Device* device) {
    std::vector<std::unique_ptr<DescriptorSetLayout> > descriptorSetLayouts;

    // Bindings set 0
    {
        VkDescriptorSetLayoutBinding bindings[] = {
            // Camera uniform buffer
            {
                bindings[0].binding = 0,
                bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                bindings[0].descriptorCount = 1 ,
                bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                bindings[0].pImmutableSamplers = nullptr // Only used for descriptorType VK_DESCRIPTOR_TYPE_SAMPLER or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
            }
        };

        descriptorSetLayouts.push_back(DescriptorSetLayout::create(device, bindings, 1));
    }

    // Bindings set 1
    {
        VkDescriptorSetLayoutBinding bindings[] = {
            // Light uniform buffer
            {
                bindings[0].binding = 0,
                bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                bindings[0].descriptorCount = 1 ,
                bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                bindings[0].pImmutableSamplers = nullptr // Only used for descriptorType VK_DESCRIPTOR_TYPE_SAMPLER or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
            }
        };
        descriptorSetLayouts.push_back(DescriptorSetLayout::create(device, bindings, 1));
    }

    if (!descriptorSetLayouts[0] || !descriptorSetLayouts[1]) {
        LUG_LOG.error("RendererVulkan: Can't create pipeline descriptor sets layout");
        return nullptr;
    }

    VkDescriptorSetLayout vkDescriptorSetLayouts[] = {
        *descriptorSetLayouts[0],
        *descriptorSetLayouts[1]
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
    VkResult result = vkCreatePipelineLayout(*device, &createInfo, nullptr, &pipelineLayout);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create pipeline layout: {}", result);
        return nullptr;
    }

    return std::unique_ptr<PipelineLayout>(new PipelineLayout(descriptorSetLayouts, pipelineLayout, device));
}

} // Vulkan
} // Graphics
} // lug
