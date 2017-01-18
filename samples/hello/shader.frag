#version 450

layout (location = 0) in vec3 verticeColor;
layout (location = 0) out vec4 color;

void main() {
    color = vec4(verticeColor, 1.0f);
}
