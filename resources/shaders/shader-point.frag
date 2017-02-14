#version 450

layout (location = 0) in vec3 verticePos;
layout (location = 1) in vec3 verticeColor;
layout (location = 2) in vec3 verticeNormal;
layout (location = 3) in vec2 verticeUv;

layout (location = 0) out vec4 color;

layout(set = 0, binding = 0) uniform cameraUniform {
    mat4 view;
    mat4 proj;
};

layout(set = 1, binding = 0) uniform lightUniform {
    vec3 lightAmbient;
    float lightConstant;
    vec3 lightDiffuse;
    float lightLinear;
    vec3 lightSpecular;
    float lightQuadric;
    vec3 lightPos;
};

vec3 getAmbient() {
    return lightAmbient * verticeColor;
}

vec3 getDiffuse() {
    vec3 fragToLight = normalize(lightPos - verticePos);
    vec3 normal = normalize(verticeNormal);
    float diff = max(dot(normal, fragToLight), 0.0);

    return diff * verticeColor * lightDiffuse;
}

void main() {

    vec3 ambient = getAmbient();
    vec3 diffuse = getDiffuse();

    float distance = length(lightPos - verticePos);
    float attenuation = 1.0f / (lightConstant + lightLinear * distance +
                lightQuadric * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;

    color = vec4(ambient + diffuse, 1.0f);
}
