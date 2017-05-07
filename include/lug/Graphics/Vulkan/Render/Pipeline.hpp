#pragma once

#include <cstdint>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

/**
* @brief      Class for the Vulkan pipeline, Render side.
*/
class LUG_GRAPHICS_API Pipeline : public Resource {
public:
    /**
     * @brief      Handle of the Pipeline.
     *             It's a concatenation of three parts: PrimitivePart, MaterialPart and PipelinePart
     *             It allows to uniquely identify a pipeline using these characteristics.
     */
    struct Handle {
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
         * @brief      Describes the material.
         */
        struct MaterialPart {
            union {
                struct {
                    uint32_t countMaterialTextures : 3;
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
                uint32_t materialPart : 3;
            };

            uint32_t value;
        };

        explicit operator uint32_t() {
            return value;
        }

        /**
         * @brief      Create a pipeline handle.
         *
         * @param[in]  primitivePart  The primitive part. It should be created manually beforehand.
         * @param[in]  materialPart   The material part. It should be created manually beforehand.
         *
         * @return     The created handle.
         */
        static Handle create(PrimitivePart primitivePart, MaterialPart materialPart) {
            Handle handle;

            handle.primitivePart = static_cast<uint32_t>(primitivePart);
            handle.materialPart = static_cast<uint32_t>(materialPart);

            return handle;
        };
    };

public:
    Pipeline() = default;

    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&&) = delete;

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&&) = delete;

    ~Pipeline() = default;
};

} // Render
} // Vulkan
} // Graphics
} // lug
