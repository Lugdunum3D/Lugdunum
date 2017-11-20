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

inline Pipeline::Id Pipeline::getSkyboxBaseId() {
    Pipeline::Id::Skybox::ExtraPart extraPart;
    extraPart.antialiasing = 0;

    return Pipeline::Id::createSkybox(extraPart);
}

inline Pipeline::Id Pipeline::getIrradianceMapBaseId() {
    return Pipeline::Id::createIrradianceMap();
}

inline Pipeline::Id Pipeline::getPrefilteredMapBaseId() {
    return Pipeline::Id::createPrefilteredMap();
}

inline Pipeline::Id Pipeline::getBrdfLutBaseId() {
    return Pipeline::Id::createBrdfLut();
}

inline const API::GraphicsPipeline& Pipeline::getPipelineAPI() {
    return _pipeline;
}
