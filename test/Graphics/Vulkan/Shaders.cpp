#include <gtest/gtest.h>
#include <vector>

#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

namespace lug {
namespace Graphics {

#if defined(ENABLE_LONG_TESTS)
static void tryToCompileShader(Render::Technique::Type technique, Vulkan::Render::Pipeline::ShaderBuilder::Type type, Vulkan::Render::Pipeline::Id id) {
    Vulkan::Render::Pipeline::ShaderBuilder::buildShader("./shaders/", technique, type, id);
}

static std::vector<Vulkan::Render::Pipeline::Id> generatePipelineIds() {
    std::vector<Vulkan::Render::Pipeline::Id> pipelineIds{};

    Vulkan::Render::Pipeline::Id::PrimitivePart primitivePart{};
    Vulkan::Render::Pipeline::Id::MaterialPart materialPart{};

    for (uint8_t positionVertexData = 0; positionVertexData <= 1; ++positionVertexData) {
        primitivePart.positionVertexData = positionVertexData;

        for (uint8_t normalVertexData = 0; normalVertexData <= 1; ++normalVertexData) {
            primitivePart.normalVertexData = normalVertexData;

            for (uint8_t tangentVertexData = 0; tangentVertexData <= 1; ++tangentVertexData) {
                primitivePart.tangentVertexData = tangentVertexData;

                for (uint8_t countTexCoord = 0; countTexCoord <= 3; ++countTexCoord) {
                    primitivePart.countTexCoord = countTexCoord;

                    if (countTexCoord == 0) {
                        materialPart.baseColorInfo = 0b11;
                        materialPart.metallicRoughnessInfo = 0b11;
                        materialPart.normalInfo = 0b11;
                        materialPart.occlusionInfo = 0b11;
                        materialPart.emissiveInfo = 0b11;

                        pipelineIds.push_back(Vulkan::Render::Pipeline::Id::create(primitivePart, materialPart));
                    } else {
                        for (uint8_t baseColorInfo = 0; baseColorInfo <= countTexCoord; ++baseColorInfo) {
                            materialPart.baseColorInfo = baseColorInfo != countTexCoord ? baseColorInfo : 0b11;

                            for (uint8_t metallicRoughnessInfo = 0; metallicRoughnessInfo <= countTexCoord; ++metallicRoughnessInfo) {
                                materialPart.metallicRoughnessInfo = metallicRoughnessInfo != countTexCoord ? metallicRoughnessInfo : 0b11;

                                for (uint8_t normalInfo = 0; normalInfo <= countTexCoord; ++normalInfo) {
                                    materialPart.normalInfo = normalInfo != countTexCoord ? normalInfo : 0b11;

                                    for (uint8_t occlusionInfo = 0; occlusionInfo <= countTexCoord; ++occlusionInfo) {
                                        materialPart.occlusionInfo = occlusionInfo != countTexCoord ? occlusionInfo : 0b11;

                                        for (uint8_t emissiveInfo = 0; emissiveInfo <= countTexCoord; ++emissiveInfo) {
                                            materialPart.emissiveInfo = emissiveInfo != countTexCoord ? emissiveInfo : 0b11;

                                            pipelineIds.push_back(Vulkan::Render::Pipeline::Id::create(primitivePart, materialPart));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return pipelineIds;
};

class VulkanShaders : public testing::TestWithParam<Vulkan::Render::Pipeline::Id> {};

TEST_P(VulkanShaders, ForwardVertex) {
    tryToCompileShader(Render::Technique::Type::Forward, Vulkan::Render::Pipeline::ShaderBuilder::Type::Vertex, GetParam());
}

TEST_P(VulkanShaders, ForwardFragment) {
    tryToCompileShader(Render::Technique::Type::Forward, Vulkan::Render::Pipeline::ShaderBuilder::Type::Fragment, GetParam());
}

INSTANTIATE_TEST_CASE_P(
    AllPipelineIds,
    VulkanShaders,
    testing::ValuesIn(generatePipelineIds()));

#endif

} // Graphics
} // lug
