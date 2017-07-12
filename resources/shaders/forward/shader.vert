#version 450

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC INPUTS
//////////////////////////////////////////////////////////////////////////////

layout (push_constant) uniform modelBlock {
    mat4 transform;
} model;

layout(std140, set = 0, binding = 0) uniform cameraBlock {
    mat4 view;
    mat4 proj;
} camera;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

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

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC OUTPUTS
//////////////////////////////////////////////////////////////////////////////

layout (location = 0) out vec3 outPositionWorldSpace;
layout (location = 1) out vec3 outNormalWorldSpace;

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF DYNAMIC OUTPUTS
//////////////////////////////////////////////////////////////////////////////

#if IN_TANGENT
layout (location = IN_TANGENT_LOCATION) out vec4 outTangent;
#endif

#if IN_UV >= 1
layout (location = IN_UV_0_LOCATION) out vec2 outUV0;
#endif
#if IN_UV >= 2
layout (location = IN_UV_1_LOCATION) out vec2 outUV1;
#endif
#if IN_UV >= 3
layout (location = IN_UV_2_LOCATION) out vec2 outUV2;
#endif

#if IN_COLOR >= 1
layout (location = IN_COLOR_0_LOCATION) out vec4 outColor0;
#endif
#if IN_COLOR >= 2
layout (location = IN_COLOR_1_LOCATION) out vec4 outColor1;
#endif
#if IN_COLOR >= 3
layout (location = IN_COLOR_2_LOCATION) out vec4 outColor2;
#endif

layout (location = IN_FREE_LOCATION) out vec3 outCameraPositionWorldSpace;

//////////////////////////////////////////////////////////////////////////////
// MAIN BLOCK
//////////////////////////////////////////////////////////////////////////////

void main() {
    //////////////////////////////////////////////////////////////////////
    // TRANSFER DYNAMIC OUTPUT
    //////////////////////////////////////////////////////////////////////

    #if IN_TANGENT
    outTangent = inTangent;
    #endif

    #if IN_UV >= 1
    outUV0 = inUV0;
    #endif
    #if IN_UV >= 2
    outUV1 = inUV1;
    #endif
    #if IN_UV >= 3
    outUV2 = inUV2;
    #endif

    #if IN_COLOR >= 1
    outColor0 = inColor0;
    #endif
    #if IN_COLOR >= 2
    outColor1 = inColor1;
    #endif
    #if IN_COLOR >= 3
    outColor2 = inColor2;
    #endif

    outCameraPositionWorldSpace = (inverse(camera.view) * vec4(vec3(0.0), 1.0)).xyz;

    //////////////////////////////////////////////////////////////////////
    // TRANSFER STATIC OUTPUT
    //////////////////////////////////////////////////////////////////////

    outPositionWorldSpace = vec3(model.transform * vec4(inPosition, 1.0));
    outNormalWorldSpace = normalize(mat3(transpose(inverse(model.transform))) * inNormal);

    //////////////////////////////////////////////////////////////////////
    // OUTPUT gl_Position
    //////////////////////////////////////////////////////////////////////

    gl_Position = camera.proj * camera.view * model.transform * vec4(inPosition, 1.0);
    gl_Position.y = -gl_Position.y;
}
