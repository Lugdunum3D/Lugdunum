// GraphicsPipeline::InputBinding


inline GraphicsPipeline::InputBinding::InputBinding(GraphicsPipeline& graphicPipelineBuilder, uint32_t idx) : _graphicPipelineBuilder(graphicPipelineBuilder), _idx(idx) {}

inline void GraphicsPipeline::InputBinding::addAttributes(VkFormat format, uint32_t offset) {
    if (_graphicPipelineBuilder._inputAttributes.size() == 0) {
        return addAttributes(0, format, offset);
    } else {
        return addAttributes(_graphicPipelineBuilder._inputAttributes.back().location + 1, format, offset);
    }
}

inline void GraphicsPipeline::InputBinding::addAttributes(uint32_t location, VkFormat format, uint32_t offset) {
    const VkVertexInputAttributeDescription inputAttribute{
        /* inputAttribute.location */ location,
        /* inputAttribute.binding */ _graphicPipelineBuilder._inputBindings[_idx].binding,
        /* inputAttribute.format */ format,
        /* inputAttribute.offset */ offset
    };

    _graphicPipelineBuilder._inputAttributes.push_back(std::move(inputAttribute));
}


// GraphicsPipeline::ViewportState


inline GraphicsPipeline::ViewportState::ViewportState(GraphicsPipeline& graphicPipelineBuilder) : _graphicPipelineBuilder(graphicPipelineBuilder) {}

inline void GraphicsPipeline::ViewportState::addViewport(const VkViewport& viewport) {
    _graphicPipelineBuilder._viewports.push_back(viewport);
}

inline void GraphicsPipeline::ViewportState::addScissor(const VkRect2D& scissor) {
    _graphicPipelineBuilder._scissors.push_back(scissor);
}


// GraphicsPipeline::RasterizationState


inline GraphicsPipeline::RasterizationState::RasterizationState(GraphicsPipeline& graphicPipelineBuilder) : _graphicPipelineBuilder(graphicPipelineBuilder) {}

inline void GraphicsPipeline::RasterizationState::enableDepthClamp() {
    _graphicPipelineBuilder._rasterizationState.depthClampEnable = VK_TRUE;
}

inline void GraphicsPipeline::RasterizationState::disableDepthClamp() {
    _graphicPipelineBuilder._rasterizationState.depthClampEnable = VK_FALSE;
}

inline void GraphicsPipeline::RasterizationState::enableRasterizerDiscard() {
    _graphicPipelineBuilder._rasterizationState.rasterizerDiscardEnable = VK_TRUE;
}

inline void GraphicsPipeline::RasterizationState::disableRasterizerDiscard() {
    _graphicPipelineBuilder._rasterizationState.rasterizerDiscardEnable = VK_FALSE;
}

inline void GraphicsPipeline::RasterizationState::setPolygonMode(VkPolygonMode polygonMode) {
    _graphicPipelineBuilder._rasterizationState.polygonMode = polygonMode;
}

inline void GraphicsPipeline::RasterizationState::setCullMode(VkCullModeFlagBits cullMode) {
    _graphicPipelineBuilder._rasterizationState.cullMode = cullMode;
}

inline void GraphicsPipeline::RasterizationState::setFrontFace(VkFrontFace frontFace) {
    _graphicPipelineBuilder._rasterizationState.frontFace = frontFace;
}

inline void GraphicsPipeline::RasterizationState::enableDepthBias(float constantFactor, float clamp, float slopeFactor) {
    _graphicPipelineBuilder._rasterizationState.depthBiasEnable = VK_TRUE;
    _graphicPipelineBuilder._rasterizationState.depthBiasConstantFactor = constantFactor;
    _graphicPipelineBuilder._rasterizationState.depthBiasClamp = clamp;
    _graphicPipelineBuilder._rasterizationState.depthBiasSlopeFactor = slopeFactor;
}

inline void GraphicsPipeline::RasterizationState::disableDepthBias() {
    _graphicPipelineBuilder._rasterizationState.depthBiasEnable = VK_FALSE;
}

inline void GraphicsPipeline::RasterizationState::setLineWidth(float lineWidth) {
    _graphicPipelineBuilder._rasterizationState.lineWidth = lineWidth;
}


// GraphicsPipeline::MultisampleState


inline GraphicsPipeline::MultisampleState::MultisampleState(GraphicsPipeline& graphicPipelineBuilder) : _graphicPipelineBuilder(graphicPipelineBuilder) {}

inline void GraphicsPipeline::MultisampleState::setRasterizationSamples(VkSampleCountFlagBits rasterizationSamples) {
    _graphicPipelineBuilder._multisampleState.rasterizationSamples = rasterizationSamples;
}

inline void GraphicsPipeline::MultisampleState::enableSampleShading() {
    _graphicPipelineBuilder._multisampleState.sampleShadingEnable = VK_TRUE;
}

inline void GraphicsPipeline::MultisampleState::disableSampleShading() {
    _graphicPipelineBuilder._multisampleState.sampleShadingEnable = VK_FALSE;
}

inline void GraphicsPipeline::MultisampleState::setMinSampleShading(float minSampleShading) {
    _graphicPipelineBuilder._multisampleState.minSampleShading = minSampleShading;
}

inline void GraphicsPipeline::MultisampleState::enableAlphaToCoverage() {
    _graphicPipelineBuilder._multisampleState.alphaToCoverageEnable = VK_TRUE;
}

inline void GraphicsPipeline::MultisampleState::disableAlphaToCoverage() {
    _graphicPipelineBuilder._multisampleState.alphaToCoverageEnable = VK_FALSE;
}

inline void GraphicsPipeline::MultisampleState::enableAlphaToOne() {
    _graphicPipelineBuilder._multisampleState.alphaToOneEnable = VK_TRUE;
}

inline void GraphicsPipeline::MultisampleState::disableAlphaToOne() {
    _graphicPipelineBuilder._multisampleState.alphaToOneEnable = VK_FALSE;
}


// GraphicsPipeline::DepthStencilState


inline GraphicsPipeline::DepthStencilState::DepthStencilState(GraphicsPipeline& graphicPipelineBuilder) : _graphicPipelineBuilder(graphicPipelineBuilder) {}

inline void GraphicsPipeline::DepthStencilState::enableDepthTest(VkCompareOp depthCompareOp) {
    _graphicPipelineBuilder._depthStencilState.depthTestEnable = VK_TRUE;
    _graphicPipelineBuilder._depthStencilState.depthCompareOp = depthCompareOp;
}

inline void GraphicsPipeline::DepthStencilState::disableDepthTest() {
    _graphicPipelineBuilder._depthStencilState.depthTestEnable = VK_FALSE;
}

inline void GraphicsPipeline::DepthStencilState::enableDepthWrite() {
    _graphicPipelineBuilder._depthStencilState.depthWriteEnable = VK_TRUE;
}

inline void GraphicsPipeline::DepthStencilState::disableDepthWrite() {
    _graphicPipelineBuilder._depthStencilState.depthWriteEnable = VK_FALSE;
}

inline void GraphicsPipeline::DepthStencilState::enableDepthBoundsTest(float minDepthBounds, float maxDepthBounds) {
    _graphicPipelineBuilder._depthStencilState.depthBoundsTestEnable = VK_TRUE;
    _graphicPipelineBuilder._depthStencilState.minDepthBounds = minDepthBounds;
    _graphicPipelineBuilder._depthStencilState.maxDepthBounds = maxDepthBounds;
}

inline void GraphicsPipeline::DepthStencilState::disableDepthBoundsTest() {
    _graphicPipelineBuilder._depthStencilState.depthBoundsTestEnable = VK_FALSE;
}

inline void GraphicsPipeline::DepthStencilState::enableStencilTest(const VkStencilOpState& front, const VkStencilOpState& back) {
    _graphicPipelineBuilder._depthStencilState.stencilTestEnable = VK_TRUE;
    _graphicPipelineBuilder._depthStencilState.front = front;
    _graphicPipelineBuilder._depthStencilState.back = back;
}

inline void GraphicsPipeline::DepthStencilState::disableStencilTest() {
    _graphicPipelineBuilder._depthStencilState.stencilTestEnable = VK_FALSE;
}


// GraphicsPipeline::ColorBlendState


inline GraphicsPipeline::ColorBlendState::ColorBlendState(GraphicsPipeline& graphicPipelineBuilder) : _graphicPipelineBuilder(graphicPipelineBuilder) {}

inline void GraphicsPipeline::ColorBlendState::enableLogicOp(VkLogicOp logicOp) {
    _graphicPipelineBuilder._colorBlendState.logicOpEnable = VK_TRUE;
    _graphicPipelineBuilder._colorBlendState.logicOp = logicOp;
}

inline void GraphicsPipeline::ColorBlendState::disableLogicOp() {
    _graphicPipelineBuilder._colorBlendState.logicOpEnable = VK_FALSE;
}

inline void GraphicsPipeline::ColorBlendState::addAttachment(const VkPipelineColorBlendAttachmentState& attachment) {
    _graphicPipelineBuilder._colorBlendAttachments.push_back(attachment);
}

inline void GraphicsPipeline::ColorBlendState::setBlendConstants(const float blendConstants[4]) {
    for (uint8_t i = 0; i < 4; ++i) {
        _graphicPipelineBuilder._colorBlendState.blendConstants[i] = blendConstants[i];
    }
}


// GraphicsPipeline


inline void GraphicsPipeline::setShader(VkShaderStageFlagBits stage, const char* entry, API::ShaderModule shaderModule) {
    const VkPipelineShaderStageCreateInfo shaderStage{
        /* shaderStage.sType */ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        /* shaderStage.pNext */ nullptr,
        /* shaderStage.flags */ 0,
        /* shaderStage.stage */ stage,
        /* shaderStage.module */ static_cast<VkShaderModule>(shaderModule),
        /* shaderStage.pName */ entry,
        /* shaderStage.pSpecializationInfo */ nullptr
    };

    _shaderStages.push_back(std::move(shaderStage));
    _shaderModules.push_back(std::move(shaderModule));
}

inline bool GraphicsPipeline::setShaderFromFile(VkShaderStageFlagBits stage, const char* entry, const std::string& filename) {
    API::Builder::ShaderModule shaderModuleBuilder(_device);

    shaderModuleBuilder.loadFromFile(filename);

    VkResult result{VK_SUCCESS};
    API::ShaderModule shaderModule;
    if (!shaderModuleBuilder.build(shaderModule, &result)) {
        LUG_LOG.error("Builder::GraphicsPipeline: Create create shader from file {}: {}", filename, result);
        return false;
    }

    setShader(stage, entry, std::move(shaderModule));
    return true;
}

inline bool GraphicsPipeline::setShaderFromData(VkShaderStageFlagBits stage, const char* entry, const std::vector<uint32_t>& data) {
    API::Builder::ShaderModule shaderModuleBuilder(_device);

    shaderModuleBuilder.loadFromData(data);

    VkResult result{VK_SUCCESS};
    API::ShaderModule shaderModule;
    if (!shaderModuleBuilder.build(shaderModule, &result)) {
        LUG_LOG.error("Builder::GraphicsPipeline: Create create shader from content: {}", result);
        return false;
    }

    setShader(stage, entry, std::move(shaderModule));
    return true;
}

inline GraphicsPipeline::InputBinding GraphicsPipeline::addInputBinding(uint32_t stride, VkVertexInputRate inputRate) {
    if (_inputBindings.size() == 0) {
        return addInputBinding(0, stride, inputRate);
    } else {
        return addInputBinding(_inputBindings.back().binding + 1, stride, inputRate);
    }
}

inline GraphicsPipeline::InputBinding GraphicsPipeline::addInputBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate) {
    const VkVertexInputBindingDescription inputBinding{
        /* inputBinding.binding */ binding,
        /* inputBinding.stride */ stride,
        /* inputBinding.inputRate */ inputRate
    };

    _inputBindings.push_back(std::move(inputBinding));
    return GraphicsPipeline::InputBinding(*this, static_cast<uint32_t>(_inputBindings.size() - 1));
}

inline void GraphicsPipeline::setInputAssemblyInfo(VkPrimitiveTopology topology, bool primitiveRestartEnable) {
    _topology = topology;
    _primitiveRestartEnable = primitiveRestartEnable;
}

inline void GraphicsPipeline::setDynamicStates(const std::set<VkDynamicState>& dynamicStates) {
    _dynamicStates = dynamicStates;
}

inline void GraphicsPipeline::setPipelineLayout(API::PipelineLayout pipelineLayout) {
    _pipelineLayout = std::move(pipelineLayout);
}

inline void GraphicsPipeline::setRenderPass(API::RenderPass renderPass, uint32_t subpass) {
    _renderPass = std::move(renderPass);
    _subpass = subpass;
}

inline void GraphicsPipeline::setPipelineCache(VkPipelineCache pipelineCache) {
    _pipelineCache = pipelineCache;
}

inline GraphicsPipeline::ViewportState GraphicsPipeline::getViewportState() {
    return GraphicsPipeline::ViewportState(*this);
}

inline GraphicsPipeline::RasterizationState GraphicsPipeline::getRasterizationState() {
    return GraphicsPipeline::RasterizationState(*this);
}

inline GraphicsPipeline::MultisampleState GraphicsPipeline::getMultisampleState() {
    return GraphicsPipeline::MultisampleState(*this);
}

inline GraphicsPipeline::DepthStencilState GraphicsPipeline::getDepthStencilState() {
    return GraphicsPipeline::DepthStencilState(*this);
}

inline GraphicsPipeline::ColorBlendState GraphicsPipeline::getColorBlendState() {
    return GraphicsPipeline::ColorBlendState(*this);
}
