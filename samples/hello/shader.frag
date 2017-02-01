#version 450

layout (location = 0) in vec3 verticePos;
layout (location = 1) in vec3 verticeColor;
layout (location = 2) in vec3 verticeNormal;
layout (location = 3) in vec2 verticeUv;

layout (location = 0) out vec4 color;

void main() {
    color = vec4(verticeColor, 1.0f);
}
