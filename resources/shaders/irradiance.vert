#version 450

layout (push_constant) uniform PushConstants {
    mat4 mvp;
} pushConstants;

layout (location = 0) in vec3 inPos;

layout (location = 0) out vec3 outPos;

void main() {
    outPos = inPos;

    gl_Position = pushConstants.mvp * vec4(inPos, 1.0);
    gl_Position.y = -gl_Position.y;
}
