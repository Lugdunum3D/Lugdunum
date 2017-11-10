#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Render/Technique/Type.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/Render/PipelineId.hpp>

namespace lug {
namespace Graphics {

namespace Vulkan {

class Renderer;

namespace Render {

/**
* @brief      Class for the Vulkan pipeline, Render side.
*/
class LUG_GRAPHICS_API Pipeline : public Resource {
public:
    enum class Type : uint8_t {
        Model,          // Able to render a model
        Skybox,         // Able to render the skybox
        IrradianceMap,  // Able to render the irradiance map
        PrefilteredMap, // Able to render the prefiltered map
        BrdfLut         // Able to render the brdf lut
    };

public:
    /**
     * @brief      Id of the Pipeline.
     *             It's a concatenation of a type and some parts depending of the type
     *             It allows to uniquely identify a pipeline using these characteristics.
     */
    struct Id {
        struct Model {
            /**
             * @brief      Describes the primitive.
             */
            struct PrimitivePart {
                union {
                    struct {
                        #define LUG_DEFINE_PART(name, bits) \
                            uint32_t name : bits;
                        LUG_PIPELINE_ID_MODEL_PRIMITIVE_PART(LUG_DEFINE_PART)
                        #undef LUG_DEFINE_PART
                    };

                    uint32_t value;
                };

                explicit operator uint32_t() {
                    return value;
                }
            };

            /**
             * @brief      Describes the material. How is the material composed,
             *             with textures, no textures, etc, to be used to construct unique
             *             pipelines.
             */
            struct MaterialPart {
                union {
                    struct {
                        #define LUG_DEFINE_PART(name, bits) \
                            uint32_t name : bits;
                        LUG_PIPELINE_ID_MODEL_MATERIAL_PART(LUG_DEFINE_PART)
                        #undef LUG_DEFINE_PART
                    };

                    uint32_t value;
                };

                explicit operator uint32_t() {
                    return value;
                }
            };

            struct ExtraPart {
                union {
                    struct {
                        #define LUG_DEFINE_PART(name, bits) \
                            uint32_t name : bits;
                        LUG_PIPELINE_ID_MODEL_EXTRA_PART(LUG_DEFINE_PART)
                        #undef LUG_DEFINE_PART
                    };

                    uint32_t value;
                };

                explicit operator uint32_t() {
                    return value;
                }
            };

            struct Info {
                uint32_t type : 3;

                #define LUG_DEFINE_PART(name, bits) \
                    uint32_t name : bits;

                LUG_PIPELINE_ID_MODEL_PRIMITIVE_PART(LUG_DEFINE_PART)
                LUG_PIPELINE_ID_MODEL_MATERIAL_PART(LUG_DEFINE_PART)
                LUG_PIPELINE_ID_MODEL_EXTRA_PART(LUG_DEFINE_PART)

                #undef LUG_DEFINE_PART
            };
        };

        union {
            struct {
                union {
                    uint32_t type : 3;

                    Model::Info modelInfo;
                };
            };

            uint32_t value;
        };

        Id(uint32_t id = 0): value(id) {}

        explicit operator uint32_t() {
            return value;
        }

        explicit operator bool() const {
            return value != 0;
        }

        bool operator==(const Id& other) const {
            return value == other.value;
        }

        bool operator!=(const Id& other) const {
            return value != other.value;
        }

        bool operator<(const Id& other) const {
            return value < other.value;
        }

        Model::PrimitivePart getModelPrimitivePart() {
            LUG_ASSERT(static_cast<Type>(type) == Type::Model, "Must be of type Model to use getModelPrimitivePart()");

            Model::PrimitivePart tmp;

            #define LUG_COPY_PRIMITIVE_PART(name, bits) \
                tmp.name = modelInfo.name;
            LUG_PIPELINE_ID_MODEL_PRIMITIVE_PART(LUG_COPY_PRIMITIVE_PART)
            #undef LUG_COPY_PRIMITIVE_PART

            return tmp;
        }

        Model::MaterialPart getModelMaterialPart() {
            LUG_ASSERT(static_cast<Type>(type) == Type::Model, "Must be of type Model to use getModelMaterialPart()");

            Model::MaterialPart tmp;

            #define LUG_COPY_MATERIAL_PART(name, bits) \
                tmp.name = modelInfo.name;
            LUG_PIPELINE_ID_MODEL_MATERIAL_PART(LUG_COPY_MATERIAL_PART)
            #undef LUG_COPY_MATERIAL_PART

            return tmp;
        }

        Model::ExtraPart getModelExtraPart() {
            LUG_ASSERT(static_cast<Type>(type) == Type::Model, "Must be of type Model to use getModelExtraPart()");

            Model::ExtraPart tmp;

            #define LUG_COPY_EXTRA_PART(name, bits) \
                tmp.name = modelInfo.name;
            LUG_PIPELINE_ID_MODEL_EXTRA_PART(LUG_COPY_EXTRA_PART)
            #undef LUG_COPY_EXTRA_PART

            return tmp;
        }

        Type getType() {
            return static_cast<Type>(type);
        }

        /**
         * @brief      Create a pipeline id for model rendering
         *
         * @param[in]  primitivePart  The primitive part. It should be created manually beforehand.
         * @param[in]  materialPart   The material part. It should be created manually beforehand.
         * @param[in]  extraPart      The extra part. It should be created manually beforehand.
         *
         * @return     The created id.
         */
        static Id createModel(Model::PrimitivePart primitivePart, Model::MaterialPart materialPart, Model::ExtraPart extraPart) {
            Id id;

            id.type = static_cast<uint8_t>(Type::Model);

            #define LUG_COPY_PRIMITIVE_PART(name, bits) \
                id.modelInfo.name = primitivePart.name;
            LUG_PIPELINE_ID_MODEL_PRIMITIVE_PART(LUG_COPY_PRIMITIVE_PART)
            #undef LUG_COPY_PRIMITIVE_PART

            #define LUG_COPY_MATERIAL_PART(name, bits) \
                id.modelInfo.name = materialPart.name;
            LUG_PIPELINE_ID_MODEL_MATERIAL_PART(LUG_COPY_MATERIAL_PART)
            #undef LUG_COPY_MATERIAL_PART

            #define LUG_COPY_EXTRA_PART(name, bits) \
                id.modelInfo.name = extraPart.name;
            LUG_PIPELINE_ID_MODEL_EXTRA_PART(LUG_COPY_EXTRA_PART)
            #undef LUG_COPY_EXTRA_PART

            return id;
        };
    };

    static_assert(sizeof(Id) <= 4, "Pipeline::Id should be less or equal to 4 bytes");

public:
    class LUG_GRAPHICS_API ShaderBuilder {
    public:
        enum class Type : uint8_t {
            Vertex,
            Fragment
        };

    public:
        ShaderBuilder() = delete;

        ShaderBuilder(const ShaderBuilder&) = delete;
        ShaderBuilder(ShaderBuilder&&) = delete;

        ShaderBuilder& operator=(const ShaderBuilder&) = delete;
        ShaderBuilder& operator=(ShaderBuilder&&) = delete;

        ~ShaderBuilder() = delete;

    public:
        static std::vector<uint32_t> buildShader(std::string shaderRoot, ::lug::Graphics::Render::Technique::Type technique, Type type, Pipeline::Id id);
        static std::vector<uint32_t> buildShaderFromFile(std::string filename, Type type, Pipeline::Id id);
        static std::vector<uint32_t> buildShaderFromString(std::string filename, std::string content, Type type, Pipeline::Id id);
    };

public:
    Pipeline(Renderer& renderer, Id id);

    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&&) = delete;

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&&) = delete;

    ~Pipeline() = default;

    /**
     * @brief      Returns the id of the Pipeline.
     *
     * @return     The id.
     */
    Id getId() const;

    static Id getModelBaseId();

    const API::GraphicsPipeline& getPipelineAPI();

    static Resource::SharedPtr<Pipeline> create(Renderer& renderer, Id id);

private:
    bool init();

    bool initModel();

private:
    Renderer& _renderer;
    Id _id;

    API::GraphicsPipeline _pipeline;
};

#include <lug/Graphics/Vulkan/Render/Pipeline.inl>

} // Render
} // Vulkan
} // Graphics
} // lug

// Make Pipeline Id hashable like a uint32_t
namespace std {
    template<> struct hash<lug::Graphics::Vulkan::Render::Pipeline::Id> {
        size_t operator()(const lug::Graphics::Vulkan::Render::Pipeline::Id& id) const {
            return hash<uint32_t>()(id.value);
        }
    };
}
