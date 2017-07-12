#version 450

//////////////////////////////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////////////////////////////

const float PI = 3.14159265359;

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC INPUTS
//////////////////////////////////////////////////////////////////////////////

layout(std140, set = 0, binding = 0) uniform cameraBlock {
    mat4 view;
    mat4 proj;
} camera;

struct Light {
    vec3 position;
    float distance;
    vec4 color;
    vec3 direction;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float falloffAngle;
    float falloffExponent;
    uint type;
};

struct Material {
    vec4 color;
    vec3 emissive;
    float metallic;
    float roughness;
    float normalTextureScale;
    float occlusionTextureStrength;
};

layout(std140, set = 1, binding = 0) uniform lightDataBlock {
    Light lights[50];
    uint lightsNb;
};

layout(std140, set = 2, binding = 0) uniform materialBlock {
    Material material;
};

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
layout (set = 3, binding = TEXTURE_COLOR_BINDING) uniform sampler2D textureColor;
#endif
#if TEXTURE_METALLIC_ROUGHNESS
layout (set = 3, binding = TEXTURE_METALLIC_ROUGHNESS_BINDING) uniform sampler2D textureMetallicRoughness;
#endif
#if TEXTURE_NORMAL
layout (set = 3, binding = TEXTURE_NORMAL_BINDING) uniform sampler2D textureNormal;
#endif
#if TEXTURE_OCCLUSION
layout (set = 3, binding = TEXTURE_OCCLUSION_BINDING) uniform sampler2D textureOcclusion;
#endif
#if TEXTURE_EMISSIVE
layout (set = 3, binding = TEXTURE_EMISSIVE_BINDING) uniform sampler2D textureEmissive;
#endif

layout (location = IN_FREE_LOCATION) in vec3 inCameraPositionWorldSpace;

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC OUTPUTS
//////////////////////////////////////////////////////////////////////////////

layout (location = 0) out vec4 outColor;

//////////////////////////////////////////////////////////////////////////////
// BRDF FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float NdotH, float roughness) {
    const float a = roughness * roughness;
    const float a2 = a * a;
    const float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float NdotL, float roughness) {
    const float r = (roughness + 1.0);
    const float k = (r * r) / 8.0;

    const float GL = NdotL / (NdotL * (1.0 - k) + k);
    const float GV = NdotV / (NdotV * (1.0 - k) + k);

    return GL * GV;
}

//////////////////////////////////////////////////////////////////////////////
// MAIN BLOCK
//////////////////////////////////////////////////////////////////////////////

void main() {
    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL COLOR
    //////////////////////////////////////////////////////////////////////

    vec4 albedo = material.color;

    #if TEXTURE_COLOR
    albedo = albedo * pow(texture(textureColor, TEXTURE_COLOR_UV).rgba, vec4(2.2));
    #endif

    #if IN_COLOR >= 1
    albedo = albedo * inColor0;
    #endif

    #if IN_COLOR >= 2
    albedo = albedo * inColor1;
    #endif

    #if IN_COLOR >= 3
    albedo = albedo * inColor2;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL METALLIC & ROUGHNESS
    //////////////////////////////////////////////////////////////////////

    #if TEXTURE_METALLIC_ROUGHNESS
    const vec3 textureValue = texture(textureMetallicRoughness, TEXTURE_METALLIC_ROUGHNESS_UV).rgb;

    const float metallic = material.metallic * textureValue.b;
    const float roughness = max(material.roughness * textureValue.g, 0.05);
    # else
    const float metallic = material.metallic;
    const float roughness = max(material.roughness, 0.05);
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL NORMAL WITH THE NORMAL MAP
    //////////////////////////////////////////////////////////////////////

    #if TEXTURE_NORMAL
    // Get the normal from the texture in tangent space and scale it
    vec3 normalTangentSpace = 2.0 * texture(textureNormal, TEXTURE_NORMAL_UV).rgb - 1.0;
    normalTangentSpace = normalize(normalTangentSpace * vec3(material.normalTextureScale, material.normalTextureScale, 1.0));

    // Compute the matrix to transform the normal from tangent space to world space
    # if IN_TANGENT
    const vec3 tangent = normalize(inTangent.xyz);
    const vec3 bitangent = normalize(cross(inNormalWorldSpace, tangent) * inTangent.w);
    # else
    const vec3 deltaPosX = dFdx(inPositionWorldSpace);
    const vec3 deltaPosY = dFdy(inPositionWorldSpace);
    const vec2 deltaUvX = dFdx(TEXTURE_NORMAL_UV);
    const vec2 deltaUvY = dFdy(TEXTURE_NORMAL_UV);

    vec3 tangent = (deltaUvY.t * deltaPosX - deltaUvX.t * deltaPosY) / (deltaUvX.s * deltaUvY.t - deltaUvY.s * deltaUvX.t);
    tangent = normalize(tangent - inNormalWorldSpace * dot(inNormalWorldSpace, tangent));
    const vec3 bitangent = normalize(cross(inNormalWorldSpace, tangent));
    # endif

    const mat3 tbn = mat3(tangent, bitangent, inNormalWorldSpace);

    // Transform the normal from tangent space to world space
    const vec3 normalWorldSpace = normalize(tbn * normalTangentSpace);
    #else
    const vec3 normalWorldSpace = inNormalWorldSpace;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL OCCLUSION
    //////////////////////////////////////////////////////////////////////

    #if TEXTURE_OCCLUSION
    const float occlusion = texture(textureOcclusion, TEXTURE_OCCLUSION_UV).r;
    #else
    const float occlusion = 1.0;
    #endif

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE FINAL EMISSIVE
    //////////////////////////////////////////////////////////////////////

    #if TEXTURE_EMISSIVE
    const vec3 emissive = material.emissive * pow(texture(textureEmissive, TEXTURE_EMISSIVE_UV).rgb, vec3(2.2));
    #else
    const vec3 emissive = material.emissive;
    #endif

    //////////////////////////////////////////////////////////////////////
    // BASIC CALCULATION REQUIRED
    //////////////////////////////////////////////////////////////////////

    const vec3 viewDirection = normalize(inCameraPositionWorldSpace - inPositionWorldSpace);

    const vec3 F0 = mix(vec3(0.04), albedo.xyz, metallic);

    //////////////////////////////////////////////////////////////////////
    // CALCULATE THE LIGHTING OF THE FRAGMENT
    //////////////////////////////////////////////////////////////////////

    vec3 Lo = vec3(0.0);
    vec3 ambient = vec3(0.0);

    for (int i = 0; i < lightsNb; ++i) {
        // Ambient Light (0) : No position + No direction
        // Direction (1) : Position + Direction + No falloffAngle
        // Point (2) : Position + No direction
        // Spotlight (3) : Position + Direction + fallOffAngle

        if (lights[i].type == 0) { // Ambient Light
            ambient += (lights[i].color * albedo).xyz;
            continue;
        }

        // Discard light if it is too far away
        const float lightDistance = distance(lights[i].position, inPositionWorldSpace);
        if (lights[i].distance != 0.0 && lightDistance > lights[i].distance) {
            continue;
        }

        // lightDirection is the direction from the fragment to the light
        const vec3 lightDirection = lights[i].type == 1 ? normalize(-lights[i].direction) : normalize(lights[i].position - inPositionWorldSpace);
        const float NdotL = clamp(dot(normalWorldSpace, lightDirection), 0.0, 1.0);

        if (dot(normalWorldSpace, lightDirection) > 0.0) {
            const vec3 halfViewLightDirection = normalize(viewDirection + lightDirection);

            const float NdotV = clamp(dot(normalWorldSpace, viewDirection), 0.0, 1.0);
            const float LdotH = clamp(dot(lightDirection, halfViewLightDirection), 0.0, 1.0);
            const float NdotH = clamp(dot(normalWorldSpace, halfViewLightDirection), 0.0, 1.0);

            // Calculate light radiance
            const float attenuation = lights[i].type == 1 ? 1.0 : lights[i].constantAttenuation + lights[i].linearAttenuation * lightDistance + lights[i].quadraticAttenuation * (lightDistance * lightDistance);
            vec3 radiance = lights[i].color.xyz / attenuation;

            // cook-torrance brdf
            const float NDF = DistributionGGX(NdotH, roughness); // Normal distribution (Distribution of the microfacets)
            const float G = GeometrySchlickGGX(NdotV, NdotL, roughness); // Geometric shadowing term (Microfacets shadowing)
            const vec3 F = fresnelSchlick(NdotH, F0); // Fresnel factor (Reflectance depending on angle of incidence)

            const vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);

            const float denominator = 4 * NdotV * NdotL;
            const vec3 specular = NDF * G * F / denominator;

            const vec3 lightFinalColor = (kD * albedo.xyz / PI + specular) * radiance * NdotL;

            // add to outgoing color to Lo
            if (lights[i].type == 3) { // Spot Light
                const float theta = dot(-lightDirection, normalize(lights[i].direction));

                if (theta > cos(lights[i].falloffAngle)) {
                    const float intensity = max(0.0, pow(theta, lights[i].falloffExponent));
                    Lo += lightFinalColor * intensity;
                }
            } else { // Direction / Point Light
                Lo += lightFinalColor;
            }
        }
    }

    vec3 color = mix(ambient, ambient * occlusion, material.occlusionTextureStrength) + Lo;

    // Tone mapping
    color = color / (color + vec3(1.0));

    color += emissive;

    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    // Final output
    outColor = vec4(color, 1.0);
}
