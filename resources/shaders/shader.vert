#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

layout (push_constant) uniform blockPushConstants {
    mat4 modelTransform; // Model transformation matrix
} pushConstants;

layout(set = 0, binding = 0) uniform cameraUniform {
    mat4 view;
    mat4 proj;
};

layout(set = 1, binding = 0) uniform testUniform {
    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
};

layout (location = 0) out vec3 verticePos;
layout (location = 1) out vec3 verticeColor;
layout (location = 2) out vec3 verticeNormal;
layout (location = 3) out vec2 verticeUv;

void main() {
    mat4 vp = proj * view;
    gl_Position = vp * pushConstants.modelTransform * vec4(pos, 1.0);
    verticePos = vec3(pushConstants.modelTransform * vec4(pos, 1.0));
    verticeColor = color;
    verticeNormal = mat3(transpose(inverse(pushConstants.modelTransform))) * normal;
    verticeUv = uv;
}
