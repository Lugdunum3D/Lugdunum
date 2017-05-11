//////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
//////////////////////////////////////////////////////////////////////////////

#version 450

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC INPUTS
//////////////////////////////////////////////////////////////////////////////

layout (push_constant) uniform modelBlock {
    mat4 transform;
} model;

layout(set = 0, binding = 0) uniform cameraBlock {
    mat4 view;
    mat4 proj;
} camera;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

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

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF STATIC OUTPUTS
//////////////////////////////////////////////////////////////////////////////

layout (location = 0) out vec3 out_world_position;
layout (location = 1) out vec3 out_world_normal;

//////////////////////////////////////////////////////////////////////////////
// BLOCK OF DYNAMIC OUTPUTS
//////////////////////////////////////////////////////////////////////////////

#if IN_TANGENT
layout (location = IN_TANGENT_LOCATION) out vec4 out_tangent;
#endif

#if IN_UV >= 1
layout (location = IN_UV_0_LOCATION) out vec2 out_uv_0;
#endif
#if IN_UV >= 2
layout (location = IN_UV_1_LOCATION) out vec2 out_uv_1;
#endif
#if IN_UV >= 3
layout (location = IN_UV_2_LOCATION) out vec2 out_uv_2;
#endif

#if IN_COLOR >= 1
layout (location = IN_COLOR_0_LOCATION) out vec4 out_color_0;
#endif
#if IN_COLOR >= 2
layout (location = IN_COLOR_1_LOCATION) out vec4 out_color_1;
#endif
#if IN_COLOR >= 3
layout (location = IN_COLOR_2_LOCATION) out vec4 out_color_2;
#endif

//////////////////////////////////////////////////////////////////////////////
// MAIN BLOCK
//////////////////////////////////////////////////////////////////////////////

void main() {
    //////////////////////////////////////////////////////////////////////
    // TRANSFER DYNAMIC OUTPUT
    //////////////////////////////////////////////////////////////////////

    #if IN_TANGENT
    out_tangent = in_tangent;
    #endif

    #if IN_UV >= 1
    out_uv_0 = in_uv_0;
    #endif
    #if IN_UV >= 2
    out_uv_1 = in_uv_1;
    #endif
    #if IN_UV >= 3
    out_uv_2 = in_uv_2;
    #endif

    #if IN_COLOR >= 1
    out_color_0 = in_color_0;
    #endif
    #if IN_COLOR >= 2
    out_color_1 = in_color_1;
    #endif
    #if IN_COLOR >= 3
    out_color_2 = in_color_2;
    #endif

    //////////////////////////////////////////////////////////////////////
    // TRANSFER STATIC OUTPUT
    //////////////////////////////////////////////////////////////////////

    out_world_position = vec3(model.transform * vec4(in_position, 1.0));
    out_world_normal = normalize(mat3(transpose(inverse(model.transform))) * in_normal);

    //////////////////////////////////////////////////////////////////////
    // OUTPUT gl_Position
    //////////////////////////////////////////////////////////////////////

    gl_Position = camera.proj * camera.view * model.transform * vec4(in_position, 1.0);
    gl_Position.y = -gl_Position.y;
}
