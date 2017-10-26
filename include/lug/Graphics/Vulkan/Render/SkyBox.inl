inline const API::GraphicsPipeline& SkyBox::getPipeline() {
    return SkyBox::_pipeline;
}

inline const lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh> SkyBox::getMesh() {
    return SkyBox::_mesh;
}

inline const lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> SkyBox::getBrdfLut() {
    return SkyBox::_brdfLut;
}
