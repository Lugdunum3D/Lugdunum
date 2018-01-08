#pragma once

/*
    positionVertexData  ///< 0 if no attribute position.
    normalVertexData    ///< 0 if no attribute normal.
    tangentVertexData   ///< 0 if no attribute tangant.
    countTexCoord       ///< The number of texcoord (maximum 3).
    countColor          ///< The number of colors (maximum 3).
    primitiveMode       ///< The primitive mode. @see Mesh::PrimitiveSet::Mode.
*/

#define LUG_PIPELINE_ID_MODEL_PRIMITIVE_PART(macro) \
    macro(positionVertexData, 1)                    \
    macro(normalVertexData, 1)                      \
    macro(tangentVertexData, 1)                     \
    macro(countTexCoord, 2)                         \
    macro(countColor, 2)                            \
    macro(primitiveMode, 3)

/*
    baseColorInfo           ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
    metallicRoughnessInfo   ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
    normalInfo              ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
    occlusionInfo           ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
    emissiveInfo            ///< 0b00 texture with UV0, 0b01 texture with UV1, 0b10 texture with UV2, 0b11 no texture.
*/

#define LUG_PIPELINE_ID_MODEL_MATERIAL_PART(macro)  \
    macro(baseColorInfo, 2)                         \
    macro(metallicRoughnessInfo, 2)                 \
    macro(normalInfo, 2)                            \
    macro(occlusionInfo, 2)                         \
    macro(emissiveInfo, 2)

/*
    displayMode         ///< Corresponding to the value in Renderer::DisplayMode.
    antialiasing        ///< Corresponding to the value in Renderer::Antialiasing.
    irradianceMapInfo   ///< 1 texture, 0 no texture.
    prefilteredMapInfo  ///< 1 texture, 0 no texture
*/

#define LUG_PIPELINE_ID_MODEL_EXTRA_PART(macro) \
    macro(displayMode, 3)                       \
    macro(antialiasing, 4)                      \
    macro(irradianceMapInfo, 1)                 \
    macro(prefilteredMapInfo, 1)

/*
    antialiasing        ///< Corresponding to the value in Renderer::Antialiasing.
*/

#define LUG_PIPELINE_ID_SKYBOX_EXTRA_PART(macro) \
    macro(antialiasing, 4)
