#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

layout (location = 0) out vec3 verticeColor;

void main() {
    mat4 viewMatrix;

    viewMatrix[0][0] = 1.81066;
    viewMatrix[0][1] = 0;
    viewMatrix[0][2] = 0;
    viewMatrix[0][3] = 0;
    viewMatrix[1][0] = 0;
    viewMatrix[1][1] = 2.07017;
    viewMatrix[1][2] = -0.515011;
    viewMatrix[1][3] = -0.514496;
    viewMatrix[2][0] = 0;
    viewMatrix[2][1] = -1.2421;
    viewMatrix[2][2] = -0.858351;
    viewMatrix[2][3] = -0.857493;
    viewMatrix[3][0] = 0;
    viewMatrix[3][1] = 0;
    viewMatrix[3][2] = 5.73669;
    viewMatrix[3][3] = 5.83095;

    gl_Position = viewMatrix * vec4(pos, 1.0);
    verticeColor = color;
}
