#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Technique/Type.hpp>
#include <lug/Graphics/Resource.hpp>

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
    /**
     * @brief      Id of the Pipeline.
     *             It's a concatenation of three parts: PrimitivePart, MaterialPart and PipelinePart
     *             It allows to uniquely identify a pipeline using these characteristics.
     */
    struct Id {
        /**
         * @brief      Describes the primitive.
         */
        struct PrimitivePart {
            union {
                struct {
                    uint32_t positionVertexData : 1;    ///< 0 if no attribute position.
                    uint32_t normalVertexData : 1;      ///< 0 if no attribute normal.
                    uint32_t tangentVertexData : 1;     ///< 0 if no attribute tangeant.
                    uint32_t countTexCoord : 2;         ///< The number of texcoord (maximum 3).
                    uint32_t primitiveMode : 3;         ///< The primitive mode. @see Mesh::PrimitiveSet::Mode.
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
                    uint32_t baseColorInfo : 2;             ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                    uint32_t metallicRoughnessInfo : 2;     ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                    uint32_t normalInfo : 2;                ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                    uint32_t occlusionInfo : 2;             ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                    uint32_t emissiveInfo : 2;              ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
                };

                uint32_t value;
            };

            explicit operator uint32_t() {
                return value;
            }
        };

        union {
            struct {
                uint32_t primitivePart : 8;
                uint32_t materialPart : 10;
            };

            uint32_t value;
        };

        explicit operator uint32_t() {
            return value;
        }

        PrimitivePart getPrimitivePart() {
            PrimitivePart tmp;
            tmp.value = primitivePart;
            return tmp;
        }

        MaterialPart getMaterialPart() {
            MaterialPart tmp;
            tmp.value = materialPart;
            return tmp;
        }

        /**
         * @brief      Create a pipeline id.
         *
         * @param[in]  primitivePart  The primitive part. It should be created manually beforehand.
         * @param[in]  materialPart   The material part. It should be created manually beforehand.
         *
         * @return     The created id.
         */
        static Id create(PrimitivePart primitivePart, MaterialPart materialPart) {
            Id id;

            id.primitivePart = static_cast<uint32_t>(primitivePart);
            id.materialPart = static_cast<uint32_t>(materialPart);

            return id;
        };
    };

public:
    class ShaderBuilder {
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
    Pipeline(Id id);

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

private:
    bool init();

private:
    static Resource::SharedPtr<Pipeline> create(Renderer& renderer, Id id);

private:
    Id _id;
};

#include <lug/Graphics/Vulkan/Render/Pipeline.inl>

} // Render
} // Vulkan
} // Graphics
} // lug

// Make Pipeline Id hashable like a uint32_t
namespace std {
    template <> struct hash<lug::Graphics::Vulkan::Render::Pipeline::Id>
    {
        size_t operator()(const lug::Graphics::Vulkan::Render::Pipeline::Id& id) const
        {
            return hash<uint32_t>()(id.value);
        }
    };
}
