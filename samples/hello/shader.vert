#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

layout (push_constant) uniform blockCamera {
    mat4 view;
} instanceCamera;

layout (location = 0) out vec3 verticeColor;

void main() {
    gl_Position = instanceCamera.view * vec4(pos, 1.0);
    verticeColor = color;
}
