#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <lug/Graphics/Vulkan/API/Builder/ShaderModule.hpp>
#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/RenderPass.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class LUG_GRAPHICS_API GraphicsPipeline {
public:
    class LUG_GRAPHICS_API InputBinding {
        friend class GraphicsPipeline;

    public:
        InputBinding(const InputBinding&) = default;
        InputBinding(InputBinding&&) = default;

        InputBinding& operator=(const InputBinding&) = default;
        InputBinding& operator=(InputBinding&&) = default;

        ~InputBinding() = default;

        // Setters
        void addAttributes(VkFormat format, uint32_t offset = 0);
        void addAttributes(uint32_t location, VkFormat format, uint32_t offset = 0);

    private:
        explicit InputBinding(GraphicsPipeline& graphicPipelineBuilder, uint32_t idx);

    private:
        GraphicsPipeline& _graphicPipelineBuilder;
        uint32_t _idx;
    };

    class LUG_GRAPHICS_API ViewportState {
        friend class GraphicsPipeline;

    public:
        ViewportState(const ViewportState&) = default;
        ViewportState(ViewportState&&) = default;

        ViewportState& operator=(const ViewportState&) = default;
        ViewportState& operator=(ViewportState&&) = default;

        ~ViewportState() = default;

        // Setters
        void addViewport(const VkViewport& viewport);
        void addScissor(const VkRect2D& scissor);

    private:
        ViewportState(GraphicsPipeline& graphicPipelineBuilder);

    private:
        GraphicsPipeline& _graphicPipelineBuilder;
    };

    class LUG_GRAPHICS_API RasterizationState {
        friend class GraphicsPipeline;

    public:
        RasterizationState(const RasterizationState&) = default;
        RasterizationState(RasterizationState&&) = default;

        RasterizationState& operator=(const RasterizationState&) = default;
        RasterizationState& operator=(RasterizationState&&) = default;

        ~RasterizationState() = default;

        // Setters
        void enableDepthClamp();
        void disableDepthClamp();

        void enableRasterizerDiscard();
        void disableRasterizerDiscard();

        void setPolygonMode(VkPolygonMode polygonMode);
        void setCullMode(VkCullModeFlagBits cullMode);
        void setFrontFace(VkFrontFace frontFace);

        void enableDepthBias(float constantFactor, float clamp, float slopeFactor);
        void disableDepthBias();

        void setLineWidth(float lineWidth);

    private:
        explicit RasterizationState(GraphicsPipeline& graphicPipelineBuilder);

    private:
        GraphicsPipeline& _graphicPipelineBuilder;
    };

    class LUG_GRAPHICS_API MultisampleState {
        friend class GraphicsPipeline;

    public:
        MultisampleState(const MultisampleState&) = default;
        MultisampleState(MultisampleState&&) = default;

        MultisampleState& operator=(const MultisampleState&) = default;
        MultisampleState& operator=(MultisampleState&&) = default;

        ~MultisampleState() = default;

        // Setters
        void setRasterizationSamples(VkSampleCountFlagBits rasterizationSamples);

        void enableSampleShading();
        void disableSampleShading();

        void setMinSampleShading(float minSampleShading);

        // TODO: Setter for pSampleMask

        void enableAlphaToCoverage();
        void disableAlphaToCoverage();

        void enableAlphaToOne();
        void disableAlphaToOne();

    private:
        explicit MultisampleState(GraphicsPipeline& graphicPipelineBuilder);

    private:
        GraphicsPipeline& _graphicPipelineBuilder;
    };

    class LUG_GRAPHICS_API DepthStencilState {
        friend class GraphicsPipeline;

    public:
        DepthStencilState(const DepthStencilState&) = default;
        DepthStencilState(DepthStencilState&&) = default;

        DepthStencilState& operator=(const DepthStencilState&) = default;
        DepthStencilState& operator=(DepthStencilState&&) = default;

        ~DepthStencilState() = default;

        // Setters
        void enableDepthTest(VkCompareOp depthCompareOp);
        void disableDepthTest();

        void enableDepthWrite(); // The depth test need to be enable also
        void disableDepthWrite();

        void enableDepthBoundsTest(float minDepthBounds, float maxDepthBounds);
        void disableDepthBoundsTest();

        void enableStencilTest(const VkStencilOpState& front, const VkStencilOpState& back);
        void disableStencilTest();

    private:
        DepthStencilState(GraphicsPipeline& graphicPipelineBuilder);

    private:
        GraphicsPipeline& _graphicPipelineBuilder;
    };

    class LUG_GRAPHICS_API ColorBlendState {
        friend class GraphicsPipeline;

    public:
        ColorBlendState(const ColorBlendState&) = default;
        ColorBlendState(ColorBlendState&&) = default;

        ColorBlendState& operator=(const ColorBlendState&) = default;
        ColorBlendState& operator=(ColorBlendState&&) = default;

        ~ColorBlendState() = default;

        // Setters
        void enableLogicOp(VkLogicOp logicOp);
        void disableLogicOp();

        void addAttachment(const VkPipelineColorBlendAttachmentState& attachment);

        void setBlendConstants(const float blendConstants[4]);

    private:
        ColorBlendState(GraphicsPipeline& graphicPipelineBuilder);

    private:
        GraphicsPipeline& _graphicPipelineBuilder;
    };

public:
    GraphicsPipeline(const API::Device& device);

    GraphicsPipeline(const GraphicsPipeline&&) = delete;
    GraphicsPipeline(GraphicsPipeline&&) = delete;

    GraphicsPipeline& operator=(const GraphicsPipeline&&) = delete;
    GraphicsPipeline& operator=(GraphicsPipeline&&) = delete;

    ~GraphicsPipeline() = default;

    // Setters
    void setShader(VkShaderStageFlagBits stage, const char* entry, API::ShaderModule shaderModule);
    bool setShaderFromFile(VkShaderStageFlagBits stage, const char* entry, const std::string& filename);
    bool setShaderFromData(VkShaderStageFlagBits stage, const char* entry, const std::vector<uint32_t>& data);

    InputBinding addInputBinding(uint32_t stride, VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
    InputBinding addInputBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);

    void setInputAssemblyInfo(VkPrimitiveTopology topology, bool primitiveRestartEnable);
    void setDynamicStates(const std::set<VkDynamicState>& dynamicStates);

    void setPipelineLayout(API::PipelineLayout pipelineLayout);
    void setRenderPass(API::RenderPass renderPass, uint32_t subpass);
    void setPipelineCache(VkPipelineCache pipelineCache);

    // Getters
    ViewportState getViewportState();
    RasterizationState getRasterizationState();
    MultisampleState getMultisampleState();
    DepthStencilState getDepthStencilState();
    ColorBlendState getColorBlendState();

    // Build methods
    bool build(API::GraphicsPipeline& graphicPipeline, VkResult* returnResult = nullptr);
    std::unique_ptr<API::GraphicsPipeline> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    // Shaders state
    std::vector<API::ShaderModule> _shaderModules;
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

    // Vertex input state
    std::vector<VkVertexInputBindingDescription> _inputBindings;
    std::vector<VkVertexInputAttributeDescription> _inputAttributes;

    // Input assembly state
    VkPrimitiveTopology _topology{VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
    bool _primitiveRestartEnable{VK_FALSE};

    // Viewport state
    VkPipelineViewportStateCreateInfo _viewportState{
        /* _viewportState.sType */ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        /* _viewportState.pNext */ nullptr,
        /* _viewportState.flags */ 0,
        /* _viewportState.viewportCount */ 0, // Will be set to _viewports.size()
        /* _viewportState.pViewports */ nullptr, // Will be set to _viewports.data()
        /* _viewportState.scissorCount */ 0, // Will be set to _scissors.size()
        /* _viewportState.pScissors */ nullptr // Will be set to _scissors.data()
    };

    std::vector<VkViewport> _viewports;
    std::vector<VkRect2D> _scissors;

    // Rasterization state
    VkPipelineRasterizationStateCreateInfo _rasterizationState{
        /* _rasterizationState.sType */ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        /* _rasterizationState.pNext */ nullptr,
        /* _rasterizationState.flags */ 0,
        /* _rasterizationState.depthClampEnable */ VK_FALSE,
        /* _rasterizationState._rasterizationStateDiscardEnable */ VK_FALSE,
        /* _rasterizationState.polygonMode */ VK_POLYGON_MODE_FILL,
        /* _rasterizationState.cullMode */ VK_CULL_MODE_BACK_BIT,
        /* _rasterizationState.frontFace */ VK_FRONT_FACE_COUNTER_CLOCKWISE,
        /* _rasterizationState.depthBiasEnable */ VK_FALSE,
        /* _rasterizationState.depthBiasConstantFactor */ 0.0f,
        /* _rasterizationState.depthBiasClamp */ 0.0f,
        /* _rasterizationState.depthBiasSlopeFactor */ 0.0f,
        /* _rasterizationState.lineWidth */ 1.0f
    };

    // Multisample state
    VkPipelineMultisampleStateCreateInfo _multisampleState{
        /* _multisampleState.sType */ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        /* _multisampleState.pNext */ nullptr,
        /* _multisampleState.flags */ 0,
        /* _multisampleState.rasterizationSamples */ VK_SAMPLE_COUNT_1_BIT,
        /* _multisampleState.sampleShadingEnable */ VK_FALSE,
        /* _multisampleState.minSampleShading */ 0.0f,
        /* _multisampleState.pSampleMask */ nullptr,
        /* _multisampleState.alphaToCoverageEnable */ VK_FALSE,
        /* _multisampleState.alphaToOneEnable */ VK_FALSE
    };

    // Depth and Stencil state
    VkPipelineDepthStencilStateCreateInfo _depthStencilState{
        /* _depthStencilState.sType */ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        /* _depthStencilState.pNext */ nullptr,
        /* _depthStencilState.flags */ 0,
        /* _depthStencilState.depthTestEnable */ VK_FALSE,
        /* _depthStencilState.depthWriteEnable */ VK_FALSE,
        /* _depthStencilState.depthCompareOp */ VK_COMPARE_OP_NEVER,
        /* _depthStencilState.depthBoundsTestEnable */ VK_FALSE,
        /* _depthStencilState.stencilTestEnable */ VK_FALSE,
        /* _depthStencilState.front */ {},
        /* _depthStencilState.back */ {},
        /* _depthStencilState.minDepthBounds */ 0.0f,
        /* _depthStencilState.maxDepthBounds */ 0.0f
    };

    // Color Blend state
    VkPipelineColorBlendStateCreateInfo _colorBlendState{
        /* _colorBlendState.sType */ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        /* _colorBlendState.pNext */ nullptr,
        /* _colorBlendState.flags */ 0,
        /* _colorBlendState.logicOpEnable */ VK_FALSE,
        /* _colorBlendState.logicOp */ VK_LOGIC_OP_CLEAR,
        /* _colorBlendState.attachmentCount */ 0, // Will be set to _colorBlendAttachments.size()
        /* _colorBlendState.pAttachments */ nullptr, // Will be set to _colorBlendAttachments.data()
        /* _colorBlendState.blendConstants */ {0.0f, 0.0f, 0.0f, 0.0f}
    };

    std::vector<VkPipelineColorBlendAttachmentState> _colorBlendAttachments;

    // Dynamic state
    std::set<VkDynamicState> _dynamicStates;

    // General
    API::PipelineLayout _pipelineLayout;
    API::RenderPass _renderPass;
    uint32_t _subpass{0};
    VkPipelineCache _pipelineCache{VK_NULL_HANDLE};
};

#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
