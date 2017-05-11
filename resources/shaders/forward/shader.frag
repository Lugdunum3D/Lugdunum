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

layout (location = 0) in vec3 in_world_position;
layout (location = 1) in vec3 in_world_normal;

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF DYNAMIC INPUTS
//////////////////////////////////////////////////////////////////////////////

#if IN_TANGENT
layout (location = IN_TANGENT_LOCATION) in vec4 in_tangent;
#endif

#if IN_UV >= 1
layout (location = IN_UV_0_LOCATION) in vec2 in_uv_0;
#endif
#if IN_UV >= 2
layout (location = IN_UV_1_LOCATION) in vec2 in_uv_1;
#endif
#if IN_UV >= 3
layout (location = IN_UV_2_LOCATION) in vec2 in_uv_2;
#endif

#if IN_COLOR >= 1
layout (location = IN_COLOR_0_LOCATION) in vec4 in_color_0;
#endif
#if IN_COLOR >= 2
layout (location = IN_COLOR_1_LOCATION) in vec4 in_color_1;
#endif
#if IN_COLOR >= 3
layout (location = IN_COLOR_2_LOCATION) in vec4 in_color_2;
#endif

#if TEXTURE_COLOR
layout (set = 2, binding = TEXTURE_COLOR_BINDING) uniform sampler2D texture_color;
#endif
#if TEXTURE_METALLIC_ROUGHNESS
layout (set = 2, binding = TEXTURE_METALLIC_ROUGHNESS_BINDING) uniform sampler2D texture_metallic_roughness;
#endif
#if TEXTURE_NORMAL
layout (set = 2, binding = TEXTURE_NORMAL_BINDING) uniform sampler2D texture_normal;
#endif
#if TEXTURE_OCCLUSION
layout (set = 2, binding = TEXTURE_OCCLUSION_BINDING) uniform sampler2D texture_occlusion;
#endif
#if TEXTURE_EMISSIVE
layout (set = 2, binding = TEXTURE_EMISSIVE_BINDING) uniform sampler2D texture_emissive;
#endif

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC OUTPUTS
//////////////////////////////////////////////////////////////////////////////

layout (location = 0) out vec4 out_color;

//////////////////////////////////////////////////////////////////////////////
// MAIN BLOCK
//////////////////////////////////////////////////////////////////////////////

void main() {
    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL COLOR
    //////////////////////////////////////////////////////////////////////

    vec4 color = material.color;

    #if TEXTURE_COLOR
    color = color * texture(texture_color, TEXTURE_COLOR_UV).rgba;
    #endif

    #if IN_COLOR >= 1
    color = color * in_color_0;
    #endif

    #if IN_COLOR >= 2
    color = color * in_color_1;
    #endif

    #if IN_COLOR >= 3
    color = color * in_color_2;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL METALLIC & ROUGHNESS
    //////////////////////////////////////////////////////////////////////

    float metallic = material.metallic;
    float roughness = material.roughness;

    #if TEXTURE_METALLIC_ROUGHNESS
    vec3 textureValue = texture(texture_metallic_roughness, TEXTURE_METALLIC_ROUGHNESS_UV).rgb;

    metallic = metallic * textureValue.b;
    roughness = roughness * textureValue.g;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL NORMAL WITH THE NORMAL MAP
    //////////////////////////////////////////////////////////////////////

    vec3 world_normal = in_world_normal;

    #if TEXTURE_NORMAL
    // Get the normal from the texture in tangent space and scale it
    vec3 normal_tangentspace = 2.0 * texture(texture_normal, TEXTURE_NORMAL_UV).rgb - 1.0;
    normal_tangentspace = normalize(normal_tangentspace * vec3(material.normalTextureScale, material.normalTextureScale, 1.0));

    // Compute the matrix to transform the normal from tangent space to world space
    # if IN_TANGENT
    vec3 tangent = normalize(in_tangent.xyz);
    vec3 bitangent = normalize(cross(in_world_normal, tangent) * in_tangent.w);
    # else
    vec3 deltaPosX = dFdx(in_world_position);
    vec3 deltaPosY = dFdy(in_world_position);
    vec2 deltaUvX = dFdx(TEXTURE_NORMAL_UV);
    vec2 deltaUvY = dFdy(TEXTURE_NORMAL_UV);

    vec3 tangent = (deltaUvY.t * deltaPosX - deltaUvX.t * deltaPosY) / (deltaUvX.s * deltaUvY.t - deltaUvY.s * deltaUvX.t);
    tangent = normalize(tangent - in_world_normal * dot(in_world_normal, tangent));
    vec3 bitangent = normalize(cross(in_world_normal, tangent));
    # endif

    mat3 tbn = mat3(tangent, bitangent, in_world_normal);

    // Transform the normal from tangent space to world space
    world_normal = normalize(tbn * normal_tangentspace);
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL OCCLUSION
    //////////////////////////////////////////////////////////////////////

    float occlusion = 1.0f;

    #if TEXTURE_OCCLUSION
    occlusion = occlusion * texture(texture_occlusion, TEXTURE_OCCLUSION_UV).r;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL EMISSIVE
    //////////////////////////////////////////////////////////////////////

    vec3 emissive = material.emissive;

    #if TEXTURE_EMISSIVE
    emissive = emissive * texture(texture_emissive, TEXTURE_EMISSIVE_UV).rgb;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE LIGHTING OF THE FRAGMENT
    //////////////////////////////////////////////////////////////////////

    // TODO: Do the calculation
    out_color = color;
    // TODO: Change to lerp between color and color * texture_occlusion with occlusionTextureStrength
}
