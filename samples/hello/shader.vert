#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

layout (push_constant) uniform blockPushConstants {
    mat4 cameraVP; // Camera view projection matrix
    mat4 modelTransform; // Model transformation matrix
} pushConstants;

layout (location = 0) out vec3 verticePos;
layout (location = 1) out vec3 verticeColor;
layout (location = 2) out vec3 verticeNormal;
layout (location = 3) out vec2 verticeUv;

void main() {
    gl_Position = pushConstants.cameraVP * pushConstants.modelTransform * vec4(pos, 1.0);
    verticePos = vec3(pushConstants.modelTransform * vec4(pos, 1.0));
    verticeColor = color;
    verticeNormal = mat3(transpose(inverse(pushConstants.modelTransform))) * normal;
    verticeUv = uv;
}
