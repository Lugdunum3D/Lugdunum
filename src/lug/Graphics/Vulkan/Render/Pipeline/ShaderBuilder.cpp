#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

#include <fstream>

#if defined(LUG_SYSTEM_ANDROID)
    #include <android/asset_manager.h>

    #include <lug/Window/Android/WindowImplAndroid.hpp>
    #include <lug/Window/Window.hpp>
#endif

#include <shaderc/shaderc.hpp>

#include <lug/System/Exception.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

std::vector<uint32_t> Pipeline::ShaderBuilder::buildShader(
    std::string shaderRoot,
    ::lug::Graphics::Render::Technique::Type technique,
    ::lug::Graphics::Renderer::DisplayMode displayMode,
    Pipeline::ShaderBuilder::Type type,
    Pipeline::Id id) {
    switch (technique) {
        case ::lug::Graphics::Render::Technique::Type::Forward:
            switch (type) {
                case Pipeline::ShaderBuilder::Type::Vertex:
                    return Pipeline::ShaderBuilder::buildShaderFromFile(shaderRoot + "forward/shader.vert", displayMode, type, id);
                case Pipeline::ShaderBuilder::Type::Fragment:
                    return Pipeline::ShaderBuilder::buildShaderFromFile(shaderRoot + "forward/shader.frag", displayMode, type, id);
            }
    }

    return {};
}

std::vector<uint32_t> Pipeline::ShaderBuilder::buildShaderFromFile(std::string filename, ::lug::Graphics::Renderer::DisplayMode displayMode, Pipeline::ShaderBuilder::Type type, Pipeline::Id id) {
#if defined(LUG_SYSTEM_ANDROID)
    // Load shader from compressed asset
    AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, filename.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        LUG_EXCEPT(FileNotFoundException, "Can't open Android asset");
        return {};
    }

    uint32_t size = AAsset_getLength(asset);

    if (size <= 0) {
        LUG_EXCEPT(FileNotFoundException, "Android asset is empty");
        return {};
    }

    std::string content;
    content.resize(size);

    AAsset_read(asset, reinterpret_cast<char*>(&content[0]), size);
    AAsset_close(asset);
#else
    std::ifstream file(filename);

    if (!file.good()) {
        LUG_EXCEPT(FileNotFoundException, "Can't find shader file");
    }

    std::string content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
#endif

    return Pipeline::ShaderBuilder::buildShaderFromString(filename, content, displayMode, type, id);
}

std::vector<uint32_t> Pipeline::ShaderBuilder::buildShaderFromString(std::string filename, std::string content, ::lug::Graphics::Renderer::DisplayMode displayMode, Pipeline::ShaderBuilder::Type type, Pipeline::Id id) {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    // Set the display mode
    options.AddMacroDefinition("DISPLAY_MODE", std::to_string(static_cast<uint8_t>(displayMode)));

    // Set macros according to the pipeline ID
    {
        Pipeline::Id::PrimitivePart primitivePart = id.getPrimitivePart();
        Pipeline::Id::MaterialPart materialPart = id.getMaterialPart();

        // Primitive part
        {
            options.AddMacroDefinition("IN_POSITION", std::to_string(primitivePart.positionVertexData));
            options.AddMacroDefinition("IN_NORMAL", std::to_string(primitivePart.normalVertexData));
            options.AddMacroDefinition("IN_TANGENT", std::to_string(primitivePart.tangentVertexData));
            options.AddMacroDefinition("IN_UV", std::to_string(primitivePart.countTexCoord));
            options.AddMacroDefinition("IN_COLOR", std::to_string(primitivePart.countColor));
        }

        // Material Part
        {
            options.AddMacroDefinition("TEXTURE_COLOR", materialPart.baseColorInfo != 0b11 ? "1" : "0");
            options.AddMacroDefinition("TEXTURE_COLOR_UV", "inUV" + std::to_string(materialPart.baseColorInfo));

            options.AddMacroDefinition("TEXTURE_METALLIC_ROUGHNESS", materialPart.metallicRoughnessInfo != 0b11 ? "1" : "0");
            options.AddMacroDefinition("TEXTURE_METALLIC_ROUGHNESS_UV", "inUV" + std::to_string(materialPart.metallicRoughnessInfo));

            options.AddMacroDefinition("TEXTURE_NORMAL", materialPart.normalInfo != 0b11 ? "1" : "0");
            options.AddMacroDefinition("TEXTURE_NORMAL_UV", "inUV" + std::to_string(materialPart.normalInfo));

            options.AddMacroDefinition("TEXTURE_OCCLUSION", materialPart.occlusionInfo != 0b11 ? "1" : "0");
            options.AddMacroDefinition("TEXTURE_OCCLUSION_UV", "inUV" + std::to_string(materialPart.occlusionInfo));

            options.AddMacroDefinition("TEXTURE_EMISSIVE", materialPart.emissiveInfo != 0b11 ? "1" : "0");
            options.AddMacroDefinition("TEXTURE_EMISSIVE_UV", "inUV" + std::to_string(materialPart.emissiveInfo));
        }

        // Set location
        {
            uint8_t location = 2;

            if (primitivePart.tangentVertexData) {
                options.AddMacroDefinition("IN_TANGENT_LOCATION", std::to_string(location++));
            }

            for (uint8_t i = 0; i < primitivePart.countTexCoord; ++i) {
                options.AddMacroDefinition("IN_UV_" + std::to_string(i) + "_LOCATION", std::to_string(location++));
            }

            for (uint8_t i = 0; i < primitivePart.countColor; ++i) {
                options.AddMacroDefinition("IN_COLOR_" + std::to_string(i) + "_LOCATION", std::to_string(location++));
            }

            options.AddMacroDefinition("IN_FREE_LOCATION", std::to_string(location++));
        }

        // Set binding
        {
            uint8_t binding = 0;

            if (materialPart.baseColorInfo != 0b11) {
                options.AddMacroDefinition("TEXTURE_COLOR_BINDING", std::to_string(binding++));
            }

            if (materialPart.metallicRoughnessInfo != 0b11) {
                options.AddMacroDefinition("TEXTURE_METALLIC_ROUGHNESS_BINDING", std::to_string(binding++));
            }

            if (materialPart.normalInfo != 0b11) {
                options.AddMacroDefinition("TEXTURE_NORMAL_BINDING", std::to_string(binding++));
            }

            if (materialPart.occlusionInfo != 0b11) {
                options.AddMacroDefinition("TEXTURE_OCCLUSION_BINDING", std::to_string(binding++));
            }

            if (materialPart.emissiveInfo != 0b11) {
                options.AddMacroDefinition("TEXTURE_EMISSIVE_BINDING", std::to_string(binding++));
            }
        }
    }

    shaderc_shader_kind kind = [](Pipeline::ShaderBuilder::Type type) {
        switch (type) {
            case Pipeline::ShaderBuilder::Type::Vertex:
                return shaderc_glsl_vertex_shader;
            case Pipeline::ShaderBuilder::Type::Fragment:
                return shaderc_glsl_fragment_shader;
        }

        return shaderc_shader_kind{};
    }(type);

    shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(content, kind, filename.c_str(), options);

    if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
        LUG_EXCEPT(InternalErrorException, module.GetErrorMessage());
    }

    std::vector<uint32_t> result(module.cbegin(), module.cend());
    return result;
}

} // Render
} // Vulkan
} // Graphics
} // lug
