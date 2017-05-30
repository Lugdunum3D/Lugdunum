#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/RenderPass.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandBuffer;
class Device;

class LUG_GRAPHICS_API Pipeline final {
public:
    explicit Pipeline(VkPipeline pipeline = VK_NULL_HANDLE, const Device* device = nullptr,
        std::unique_ptr<PipelineLayout> layout = nullptr, std::unique_ptr<RenderPass> renderPass = nullptr);

    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& pipeline);

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&& pipeline);

    ~Pipeline();

    explicit operator VkPipeline() const {
        return _pipeline;
    }

    void bind(const CommandBuffer* commandBuffer);

    void destroy();

    RenderPass* getRenderPass() const;
    PipelineLayout* getLayout() const;

    static std::unique_ptr<Pipeline> createGraphicsPipeline(const Device* device, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, VkFormat colorFormat);

private:
    VkPipeline _pipeline{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::unique_ptr<PipelineLayout> _layout;
    std::unique_ptr<RenderPass> _renderPass;
};

} // API
} // Vulkan
} // Graphics
} // lug
