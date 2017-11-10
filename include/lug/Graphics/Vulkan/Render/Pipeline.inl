inline Pipeline::Id Pipeline::getId() const {
    return _id;
}

inline Pipeline::Id Pipeline::getModelBaseId() {
    Pipeline::Id::Model::PrimitivePart primitivePart;
    primitivePart.positionVertexData = 1;
    primitivePart.normalVertexData = 1;
    primitivePart.tangentVertexData = 0;
    primitivePart.countTexCoord = 0;
    primitivePart.countColor = 0;
    primitivePart.primitiveMode = 4; // Triangles

    Pipeline::Id::Model::MaterialPart materialPart;
    materialPart.baseColorInfo = 0b11; // No texture
    materialPart.metallicRoughnessInfo = 0b11; // No texture
    materialPart.normalInfo = 0b11; // No texture
    materialPart.occlusionInfo = 0b11; // No texture
    materialPart.emissiveInfo = 0b11; // No texture

    Pipeline::Id::Model::ExtraPart extraPart;
    extraPart.displayMode = 0;
    extraPart.antialiasing = 0;
    extraPart.irradianceMapInfo = 0; // No texture
    extraPart.prefilteredMapInfo = 0; // No texture

    return Pipeline::Id::createModel(primitivePart, materialPart, extraPart);
}

inline const API::GraphicsPipeline& Pipeline::getPipelineAPI() {
    return _pipeline;
}
