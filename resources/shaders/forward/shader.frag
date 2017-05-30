//////////////////////////////////////////////////////////////////////////////
// FRAGMENT SHADER
//////////////////////////////////////////////////////////////////////////////

#version 450

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC INPUTS
//////////////////////////////////////////////////////////////////////////////

layout(set = 0, binding = 0) uniform cameraBlock {
    mat4 view;
    mat4 proj;
} camera;

layout(set = 1, binding = 0) uniform lightBlock {
  // TODO: Fill this structure
  float tmp;
} light;

layout(set = 2, binding = 0) uniform materialBlock {
    vec4 color;
    vec3 emissive;
    float metallic;
    float roughness;
    float normalTextureScale;
    float occlusionTextureStrength;
} material;

layout (location = 0) in vec3 inPositionWorldSpace;
layout (location = 1) in vec3 inNormalWorldSpace;

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF DYNAMIC INPUTS
//////////////////////////////////////////////////////////////////////////////

#if IN_TANGENT
layout (location = IN_TANGENT_LOCATION) in vec4 inTangent;
#endif

#if IN_UV >= 1
layout (location = IN_UV_0_LOCATION) in vec2 inUV0;
#endif
#if IN_UV >= 2
layout (location = IN_UV_1_LOCATION) in vec2 inUV1;
#endif
#if IN_UV >= 3
layout (location = IN_UV_2_LOCATION) in vec2 inUV2;
#endif

#if IN_COLOR >= 1
layout (location = IN_COLOR_0_LOCATION) in vec4 inColor0;
#endif
#if IN_COLOR >= 2
layout (location = IN_COLOR_1_LOCATION) in vec4 inColor1;
#endif
#if IN_COLOR >= 3
layout (location = IN_COLOR_2_LOCATION) in vec4 inColor2;
#endif

#if TEXTURE_COLOR
layout (set = 2, binding = TEXTURE_COLOR_BINDING) uniform sampler2D textureColor;
#endif
#if TEXTURE_METALLIC_ROUGHNESS
layout (set = 2, binding = TEXTURE_METALLIC_ROUGHNESS_BINDING) uniform sampler2D textureMetallicRoughness;
#endif
#if TEXTURE_NORMAL
layout (set = 2, binding = TEXTURE_NORMAL_BINDING) uniform sampler2D textureNormal;
#endif
#if TEXTURE_OCCLUSION
layout (set = 2, binding = TEXTURE_OCCLUSION_BINDING) uniform sampler2D textureOcclusion;
#endif
#if TEXTURE_EMISSIVE
layout (set = 2, binding = TEXTURE_EMISSIVE_BINDING) uniform sampler2D textureEmissive;
#endif

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC OUTPUTS
//////////////////////////////////////////////////////////////////////////////

layout (location = 0) out vec4 outColor;

//////////////////////////////////////////////////////////////////////////////
// MAIN BLOCK
//////////////////////////////////////////////////////////////////////////////

void main() {
    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL COLOR
    //////////////////////////////////////////////////////////////////////

    vec4 color = material.color;

    #if TEXTURE_COLOR
    color = color * texture(textureColor, TEXTURE_COLOR_UV).rgba;
    #endif

    #if IN_COLOR >= 1
    color = color * inColor0;
    #endif

    #if IN_COLOR >= 2
    color = color * inColor1;
    #endif

    #if IN_COLOR >= 3
    color = color * inColor2;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL METALLIC & ROUGHNESS
    //////////////////////////////////////////////////////////////////////

    float metallic = material.metallic;
    float roughness = material.roughness;

    #if TEXTURE_METALLIC_ROUGHNESS
    vec3 textureValue = texture(textureMetallicRoughness, TEXTURE_METALLIC_ROUGHNESS_UV).rgb;

    metallic = metallic * textureValue.b;
    roughness = roughness * textureValue.g;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL NORMAL WITH THE NORMAL MAP
    //////////////////////////////////////////////////////////////////////

    vec3 normalWorldSpace = inNormalWorldSpace;

    #if TEXTURE_NORMAL
    // Get the normal from the texture in tangent space and scale it
    vec3 normalTangentSpace = 2.0 * texture(textureNormal, TEXTURE_NORMAL_UV).rgb - 1.0;
    normalTangentSpace = normalize(normalTangentSpace * vec3(material.normalTextureScale, material.normalTextureScale, 1.0));

    // Compute the matrix to transform the normal from tangent space to world space
    # if IN_TANGENT
    vec3 tangent = normalize(inTangent.xyz);
    vec3 bitangent = normalize(cross(inNormalWorldSpace, tangent) * inTangent.w);
    # else
    vec3 deltaPosX = dFdx(inPositionWorldSpace);
    vec3 deltaPosY = dFdy(inPositionWorldSpace);
    vec2 deltaUvX = dFdx(TEXTURE_NORMAL_UV);
    vec2 deltaUvY = dFdy(TEXTURE_NORMAL_UV);

    vec3 tangent = (deltaUvY.t * deltaPosX - deltaUvX.t * deltaPosY) / (deltaUvX.s * deltaUvY.t - deltaUvY.s * deltaUvX.t);
    tangent = normalize(tangent - inNormalWorldSpace * dot(inNormalWorldSpace, tangent));
    vec3 bitangent = normalize(cross(inNormalWorldSpace, tangent));
    # endif

    mat3 tbn = mat3(tangent, bitangent, inNormalWorldSpace);

    // Transform the normal from tangent space to world space
    normalWorldSpace = normalize(tbn * normalTangentSpace);
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL OCCLUSION
    //////////////////////////////////////////////////////////////////////

    float occlusion = 1.0f;

    #if TEXTURE_OCCLUSION
    occlusion = occlusion * texture(textureOcclusion, TEXTURE_OCCLUSION_UV).r;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL EMISSIVE
    //////////////////////////////////////////////////////////////////////

    vec3 emissive = material.emissive;

    #if TEXTURE_EMISSIVE
    emissive = emissive * texture(textureEmissive, TEXTURE_EMISSIVE_UV).rgb;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE LIGHTING OF THE FRAGMENT
    //////////////////////////////////////////////////////////////////////

    // TODO: Do the calculation
    outColor = color;
    // TODO: Change to lerp between color and color * textureOcclusion with occlusionTextureStrength
}
